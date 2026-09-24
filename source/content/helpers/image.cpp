#include <content/helpers/image.hpp>

#include <algorithm>
#include <limits>

// stb_image ships with the ImGui backend that comes with the ImHex SDK. libimhex keeps its own
// copy internal to its texture loader, so we compile a private one here to be able to decode on
// a worker thread without touching the GPU.
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_STATIC
#define STBI_NO_STDIO
// Only BMP input for now. Drop this line (and the magic check below) to accept every format stb knows
#define STBI_ONLY_BMP
#include <stb_image.h>

namespace hex::plugin::stegoooo {

    std::optional<Image> decodeImage(std::span<const u8> data, std::string &error) {
        if (data.empty()) {
            error = "No data on the input";
            return std::nullopt;
        }

        if (data.size() < 2 || data[0] != 'B' || data[1] != 'M') {
            error = "Only BMP files are supported for now";
            return std::nullopt;
        }

        int width = 0, height = 0, sourceChannels = 0;
        auto *pixels = stbi_load_from_memory(
            data.data(),
            int(std::min<size_t>(data.size(), size_t(std::numeric_limits<int>::max()))),
            &width, &height, &sourceChannels,
            STBI_rgb_alpha
        );

        if (pixels == nullptr) {
            const auto *reason = stbi_failure_reason();
            error = reason != nullptr ? reason : "Unknown decoding error";
            return std::nullopt;
        }

        Image image;
        image.width  = u32(width);
        image.height = u32(height);
        image.rgba.assign(pixels, pixels + image.pixelCount() * 4);

        stbi_image_free(pixels);

        return image;
    }

}
