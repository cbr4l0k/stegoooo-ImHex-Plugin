#include <content/nodes.hpp>

#include <hex/api/content_registry/data_processor.hpp>
#include <hex/data_processor/attribute.hpp>
#include <hex/data_processor/node.hpp>

#include <hex/helpers/fmt.hpp>
#include <hex/helpers/scaling.hpp>
#include <hex/ui/imgui_imhex_extensions.h>

#include <imgui.h>
#include <nlohmann/json.hpp>

#include <algorithm>
#include <mutex>
#include <optional>
#include <random>
#include <span>
#include <string>
#include <string_view>
#include <vector>

namespace hex::plugin::stegoooo {

    namespace {

        enum class RandomFormat : u8 {
            /// Lowercase hex text, like `openssl rand -hex N` (2 characters per byte, no trailing newline)
            Hex = 0,
            /// The bytes themselves
            Raw = 1
        };

        /**
         * @brief Fills a buffer from std::random_device
         *
         * The standard only promises random_device is non-deterministic if the platform allows it; it makes no
         * cryptographic guarantee. libstdc++ on Linux backs it with RDSEED/RDRAND or getentropy()//dev/urandom.
         */
        std::vector<u8> generateBytes(size_t count) {
            std::random_device device;
            std::vector<u8> bytes(count);

            for (size_t i = 0; i < count; i += 4) {
                const auto value = u32(device());
                for (size_t j = 0; j < 4 && i + j < count; j += 1)
                    bytes[i + j] = u8(value >> (j * 8));
            }

            return bytes;
        }

        std::string toHex(std::span<const u8> bytes) {
            constexpr std::string_view Digits = "0123456789abcdef";

            std::string hex;
            hex.reserve(bytes.size() * 2);
            for (const auto byte : bytes) {
                hex.push_back(Digits[byte >> 4]);
                hex.push_back(Digits[byte & 0x0F]);
            }

            return hex;
        }

        std::optional<std::vector<u8>> fromHex(std::string_view hex) {
            const auto nibble = [](char c) -> int {
                if (c >= '0' && c <= '9') return c - '0';
                if (c >= 'a' && c <= 'f') return c - 'a' + 10;
                if (c >= 'A' && c <= 'F') return c - 'A' + 10;
                return -1;
            };

            if (hex.size() % 2 != 0)
                return std::nullopt;

            std::vector<u8> bytes(hex.size() / 2);
            for (size_t i = 0; i < bytes.size(); i += 1) {
                const auto high = nibble(hex[i * 2]);
                const auto low  = nibble(hex[i * 2 + 1]);
                if (high < 0 || low < 0)
                    return std::nullopt;
                bytes[i] = u8((high << 4) | low);
            }

            return bytes;
        }

    }

    /**
     * @brief Produces random bytes, the equivalent of `openssl rand -hex N`, using only the C++ standard library
     *
     * The bytes are generated when the node is created or Regenerate is pressed, never in process(): the graph is
     * re-evaluated all the time, and a message that changed on every run could never be retrieved again. They are
     * saved with the project so a saved graph reproduces the same data.
     */
    class NodeRandomBytes : public dp::Node {
    public:
        NodeRandomBytes() : Node("Random bytes", {
            dp::Attribute(dp::Attribute::IOType::Out, dp::Attribute::Type::Buffer, "Data")
        }) {
            m_bytes = generateBytes(size_t(m_count));
        }

        void process() override {
            std::vector<u8> bytes;
            RandomFormat format;
            {
                const std::scoped_lock lock(m_mutex);
                bytes  = m_bytes;
                format = m_format;
            }

            if (format == RandomFormat::Hex) {
                const auto hex = toHex(bytes);
                this->setBufferOnOutput(0, std::span(reinterpret_cast<const u8*>(hex.data()), hex.size()));
            } else {
                this->setBufferOnOutput(0, bytes);
            }
        }

        void store(nlohmann::json &j) const override {
            const std::scoped_lock lock(m_mutex);
            j["count"]  = m_count;
            j["format"] = u8(m_format);
            j["data"]   = toHex(m_bytes);
        }

        void load(const nlohmann::json &j) override {
            auto count  = DefaultCount;
            auto format = RandomFormat::Hex;
            std::optional<std::vector<u8>> data;

            // value() only falls back when a key is missing, a key of the wrong type throws
            try {
                count  = std::clamp(j.value("count", DefaultCount), MinCount, MaxCount);
                format = RandomFormat(std::clamp(j.value("format", int(RandomFormat::Hex)), 0, 1));

                // Check the length before decoding, so a bloated project file can't make us allocate for nothing
                if (const auto it = j.find("data"); it != j.end() && it->is_string() && it->get_ref<const std::string &>().size() == size_t(count) * 2)
                    data = fromHex(it->get_ref<const std::string &>());
            } catch (const nlohmann::json::exception &) {
                count  = DefaultCount;
                format = RandomFormat::Hex;
                data.reset();
            }

            auto bytes = data.has_value() ? std::move(*data) : generateBytes(size_t(count));

            const std::scoped_lock lock(m_mutex);
            m_count  = count;
            m_format = format;
            m_bytes  = std::move(bytes);
        }

    protected:
        void drawNode() override {
            // Work on copies so the (slow) generation never holds the lock process() needs
            int count;
            RandomFormat format;
            size_t currentSize;
            std::string preview;
            {
                const std::scoped_lock lock(m_mutex);
                count       = m_count;
                format      = m_format;
                currentSize = m_bytes.size();
                preview     = toHex(std::span(m_bytes).first(std::min(PreviewBytes, m_bytes.size())));
            }

            bool regenerate = false;
            ImGui::PushItemWidth(200_scaled);
            {
                if (ImGui::InputInt("Bytes", &count, 1, 1024))
                    count = std::clamp(count, MinCount, MaxCount);

                int formatIndex = int(format);
                if (ImGui::Combo("Format", &formatIndex, "Hex text\0Raw bytes\0"))
                    format = RandomFormat(formatIndex);
            }
            ImGui::PopItemWidth();

            if (ImGui::Button("Regenerate"))
                regenerate = true;

            // Only a real change of size regenerates, so clamping at the limit doesn't throw the data away
            if (size_t(count) != currentSize)
                regenerate = true;

            if (regenerate) {
                auto bytes = generateBytes(size_t(count));
                preview = toHex(std::span(bytes).first(std::min(PreviewBytes, bytes.size())));
                currentSize = bytes.size();

                const std::scoped_lock lock(m_mutex);
                m_bytes = std::move(bytes);
            }

            {
                const std::scoped_lock lock(m_mutex);
                m_count  = count;
                m_format = format;
            }

            ImGuiExt::TextFormatted("Output: {} bytes", format == RandomFormat::Hex ? currentSize * 2 : currentSize);
            ImGuiExt::TextFormatted("{}{}", preview, currentSize > PreviewBytes ? "..." : "");

            // 0 means the implementation can't vouch for the source; libstdc++ reports it when it falls back to a PRNG
            static const bool noEntropy = std::random_device().entropy() == 0.0;
            if (noEntropy)
                ImGuiExt::TextFormatted("Warning: std::random_device reports no entropy, it may be deterministic here");
        }

    private:
        constexpr static int DefaultCount = 32;
        constexpr static int MinCount     = 1;
        // std::random_device yields 4 bytes per call; 1 MiB takes about 0.4 s, 16 MiB about 7 s
        constexpr static int MaxCount     = 1024 * 1024;
        constexpr static size_t PreviewBytes = 16;

        // Edited on the render thread, read by process() on the worker thread
        mutable std::mutex m_mutex;
        int m_count = DefaultCount;
        RandomFormat m_format = RandomFormat::Hex;
        std::vector<u8> m_bytes;
    };

    void registerRandomNodes() {
        ContentRegistry::DataProcessor::add<NodeRandomBytes>("Stegoooo", "Random bytes");
    }

}
