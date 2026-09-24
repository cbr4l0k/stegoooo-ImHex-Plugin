#pragma once

#include <hex.hpp>

#include <content/helpers/image.hpp>

#include <vector>

namespace hex::plugin::stegoooo {

    /**
     * @brief Encodes an RGBA image as an uncompressed 24-bit BMP
     */
    [[nodiscard]] std::vector<u8> encodeBMP(const Image &image);

}
