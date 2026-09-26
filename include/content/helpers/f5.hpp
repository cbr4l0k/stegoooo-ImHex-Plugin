#pragma once

#include <hex.hpp>

#include <content/helpers/image.hpp>

#include <optional>
#include <span>
#include <string>
#include <vector>

namespace hex::plugin::stegoooo {

    enum class F5Channels : u8 {
        Luma = 0,
        All  = 1
    };

    struct F5Settings {
        int quality = 75;
        std::string password;
        F5Channels channels = F5Channels::All;
    };

    struct F5Stats {
        u32 k = 0;
        u32 squashMargin = 0;
        size_t usableCoefficients = 0;
        size_t changes = 0;
        size_t shrinkage = 0;
        size_t messageBits = 0;
        size_t repairedBlocks = 0;
    };

    [[nodiscard]] std::optional<Image> f5Embed(const Image &cover, std::span<const u8> message, const F5Settings &settings, F5Stats &stats, std::string &error);
    [[nodiscard]] std::optional<std::vector<u8>> f5Extract(const Image &stego, const F5Settings &settings, std::string &error);
    [[nodiscard]] std::optional<Image> jpegQuantize(const Image &cover, int quality, std::string &error, u32 margin = 16);
    [[nodiscard]] size_t f5Capacity(const Image &cover, const F5Settings &settings);

}
