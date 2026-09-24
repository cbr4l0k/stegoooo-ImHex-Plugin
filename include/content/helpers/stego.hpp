#pragma once

#include <hex.hpp>

#include <content/helpers/image.hpp>

#include <optional>
#include <span>
#include <string>
#include <vector>

namespace hex::plugin::stegoooo {

    [[nodiscard]] size_t lsbCapacity(const Image &image);
    [[nodiscard]] std::optional<Image> lsbEmbed(const Image &image, std::span<const u8> message, std::string &error);
    [[nodiscard]] std::optional<std::vector<u8>> lsbExtract(const Image &image, std::string &error);

    [[nodiscard]] double mse(const Image &original, const Image &modified);
    [[nodiscard]] double psnr(const Image &original, const Image &modified);
    [[nodiscard]] double ssim(const Image &original, const Image &modified);

}
