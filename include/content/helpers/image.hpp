#pragma once

#include <hex.hpp>

#include <optional>
#include <span>
#include <string>
#include <vector>

namespace hex::plugin::stegoooo {

    /**
     * @brief An image decoded into tightly packed 8-bit RGBA pixels, top row first.
     */
    struct Image {
        u32 width  = 0;
        u32 height = 0;
        std::vector<u8> rgba;

        [[nodiscard]] size_t pixelCount() const { return size_t(this->width) * size_t(this->height); }
        [[nodiscard]] bool isValid()      const { return this->pixelCount() > 0 && this->rgba.size() == this->pixelCount() * 4; }
    };

    /**
     * @brief Decodes a BMP file into RGBA pixels (other formats are rejected for now)
     * @param data The raw bytes of the image file
     * @param error Receives the reason for the failure if decoding didn't succeed
     * @return The decoded image, or nothing if the data isn't a supported image
     *
     * @note Pure CPU work, no GPU state is touched. Safe to call from a Data Processor worker thread.
     */
    [[nodiscard]] std::optional<Image> decodeImage(std::span<const u8> data, std::string &error);

}
