#pragma once

#include <hex.hpp>

#include <content/helpers/image.hpp>
#include <content/helpers/wav.hpp>

#include <optional>
#include <span>
#include <string>
#include <vector>

namespace hex::plugin::stegoooo {

    [[nodiscard]] size_t lsbCapacity(const Image &image);
    [[nodiscard]] std::optional<Image> lsbEmbed(const Image &image, std::span<const u8> message, std::string &error);
    [[nodiscard]] std::optional<std::vector<u8>> lsbExtract(const Image &image, std::string &error);

    // WAV covers: one bit per PCM sample. Embedding returns the complete stego WAV file.
    [[nodiscard]] size_t lsbWavCapacity(const Wav &wav);
    [[nodiscard]] std::optional<std::vector<u8>> lsbWavEmbed(const Wav &wav, std::span<const u8> message, std::string &error);
    [[nodiscard]] std::optional<std::vector<u8>> lsbWavExtract(const Wav &wav, std::string &error);

    [[nodiscard]] double mse(const Image &original, const Image &modified);
    [[nodiscard]] double psnr(const Image &original, const Image &modified);
    [[nodiscard]] double ssim(const Image &original, const Image &modified);

    // WAV metrics compare samples in their native units; both files must share format and length
    [[nodiscard]] double mse(const Wav &original, const Wav &modified);
    [[nodiscard]] double psnr(const Wav &original, const Wav &modified);
    [[nodiscard]] double ssim(const Wav &original, const Wav &modified);

}
