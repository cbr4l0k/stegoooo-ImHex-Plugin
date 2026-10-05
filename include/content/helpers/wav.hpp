#pragma once

#include <hex.hpp>

#include <optional>
#include <span>
#include <string>
#include <vector>

namespace hex::plugin::stegoooo {

    /**
     * @brief A validated RIFF/WAVE file. The original bytes are kept untouched so
     * samples can be edited in place without re-encoding the container.
     */
    struct Wav {
        std::vector<u8> bytes;
        u16 formatTag     = 0;
        u16 channels      = 0;
        u16 bitsPerSample = 0;
        u16 blockAlign    = 0;
        u32 sampleRate    = 0;
        size_t dataOffset = 0;
        size_t dataSize   = 0;

        [[nodiscard]] size_t bytesPerSample() const { return this->bitsPerSample / 8; }
        [[nodiscard]] size_t sampleCount() const { return this->dataSize / this->bytesPerSample(); }
        [[nodiscard]] size_t frameCount() const { return this->dataSize / this->blockAlign; }
    };

    /**
     * @brief Parses a little-endian RIFF/WAVE file with PCM (u8/s16/s24/s32) or IEEE float32 samples
     */
    [[nodiscard]] std::optional<Wav> decodeWav(std::span<const u8> data, std::string &error);

    /**
     * @brief Interleaved samples in their native units (u8 as 0..255, signed PCM as-is, float as stored)
     */
    [[nodiscard]] std::vector<double> wavSamples(const Wav &wav);

    /**
     * @brief Full-scale peak-to-peak range of the samples: 2^bits - 1 for PCM, 2.0 for float
     */
    [[nodiscard]] double wavSampleRange(const Wav &wav);

}
