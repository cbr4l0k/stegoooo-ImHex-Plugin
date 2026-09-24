#pragma once

#include <hex.hpp>

#include <content/helpers/image.hpp>

#include <vector>

namespace hex::plugin::stegoooo {

    enum class RGBChannel : u8 {
        R = 0,
        G = 1,
        B = 2
    };

    enum class YCbCrChannel : u8 {
        Y  = 0,
        Cb = 1,
        Cr = 2
    };

    /**
     * @brief How a single channel is turned into something to look at
     */
    enum class ChannelPreview : u8 {
        /// The channel's values straight up as brightness
        Grayscale = 0,
        /// The channel's values with the two other channels held at neutral, so Cb reads blue-yellow and Cr red-cyan
        Colorized = 1
    };

    /**
     * @brief The three 8-bit planes of an image in the YCbCr color space
     *
     * Uses the full-range BT.601 transform (the one JPEG uses), so all three planes cover 0..255
     * and the round trip back to RGB is lossless apart from rounding.
     */
    struct YCbCrPlanes {
        u32 width  = 0;
        u32 height = 0;
        std::vector<u8> y, cb, cr;

        [[nodiscard]] size_t pixelCount() const { return size_t(this->width) * size_t(this->height); }
        [[nodiscard]] bool isValid()      const { return this->pixelCount() > 0 && this->y.size() == this->pixelCount(); }

        [[nodiscard]] const std::vector<u8>& getChannel(YCbCrChannel channel) const;
    };

    /**
     * @brief Splits an RGBA image into its Y, Cb and Cr planes, dropping the alpha channel
     */
    [[nodiscard]] YCbCrPlanes toYCbCr(const Image &image);

    /**
     * @brief Expands one plane back into an RGBA buffer that can be uploaded as a texture
     */
    [[nodiscard]] std::vector<u8> renderChannel(const YCbCrPlanes &planes, YCbCrChannel channel, ChannelPreview preview);

    /**
     * @brief Expands one RGB channel into an RGBA preview buffer
     */
    [[nodiscard]] std::vector<u8> renderRGBChannel(const Image &image, RGBChannel channel, ChannelPreview preview);

    /**
     * @brief Converts an image to full-range BT.601 luma values
     */
    [[nodiscard]] std::vector<u8> toLuma(const Image &image);

}
