#include <content/helpers/wav.hpp>

#include <hex/helpers/fmt.hpp>

#include <algorithm>
#include <bit>
#include <cmath>
#include <cstring>

namespace hex::plugin::stegoooo {

    namespace {

        constexpr u16 FormatPCM        = 0x0001;
        constexpr u16 FormatIEEEFloat  = 0x0003;
        constexpr u16 FormatExtensible = 0xFFFE;

        u32 getLE(std::span<const u8> data, size_t offset, size_t bytes) {
            u32 value = 0;
            for (size_t i = 0; i < bytes; i += 1)
                value |= u32(data[offset + i]) << (i * 8);
            return value;
        }

        bool hasTag(std::span<const u8> data, size_t offset, const char *tag) {
            return std::memcmp(data.data() + offset, tag, 4) == 0;
        }

    }

    std::optional<Wav> decodeWav(std::span<const u8> data, std::string &error) {
        if (data.size() < 12 || !hasTag(data, 0, "RIFF") || !hasTag(data, 8, "WAVE")) {
            error = "Not a RIFF/WAVE file";
            return std::nullopt;
        }

        // Some writers leave a wrong RIFF size, so never trust it past the buffer end
        const auto riffEnd = std::min<size_t>(data.size(), size_t(8) + getLE(data, 4, 4));

        Wav wav;
        bool hasFormat = false, hasData = false;

        // Each chunk: 4-byte id, u32 LE size, payload, one pad byte when the size is odd
        for (size_t offset = 12; offset + 8 <= riffEnd; ) {
            const auto size  = size_t(getLE(data, offset + 4, 4));
            const auto body  = offset + 8;
            if (size > riffEnd - body) {
                error = "Truncated WAV chunk";
                return std::nullopt;
            }

            if (hasTag(data, offset, "fmt ")) {
                if (hasFormat || size < 16) {
                    error = "Invalid 'fmt ' chunk";
                    return std::nullopt;
                }
                wav.formatTag     = u16(getLE(data, body + 0, 2));
                wav.channels      = u16(getLE(data, body + 2, 2));
                wav.sampleRate    = getLE(data, body + 4, 4);
                wav.blockAlign    = u16(getLE(data, body + 12, 2));
                wav.bitsPerSample = u16(getLE(data, body + 14, 2));
                hasFormat = true;
            } else if (hasTag(data, offset, "data")) {
                if (hasData) {
                    error = "Multiple 'data' chunks are not supported";
                    return std::nullopt;
                }
                wav.dataOffset = body;
                wav.dataSize   = size;
                hasData = true;
            }

            offset = body + size + (size & 1);
        }

        if (!hasFormat || !hasData) {
            error = "WAV file is missing its 'fmt ' or 'data' chunk";
            return std::nullopt;
        }

        if (wav.formatTag == FormatExtensible) {
            error = "WAVE_FORMAT_EXTENSIBLE is not supported yet";
            return std::nullopt;
        }
        const bool isPCM   = wav.formatTag == FormatPCM && (wav.bitsPerSample == 8 || wav.bitsPerSample == 16 || wav.bitsPerSample == 24 || wav.bitsPerSample == 32);
        const bool isFloat = wav.formatTag == FormatIEEEFloat && wav.bitsPerSample == 32;
        if (!isPCM && !isFloat) {
            error = fmt::format("Unsupported WAV encoding (format tag {}, {} bits)", wav.formatTag, wav.bitsPerSample);
            return std::nullopt;
        }

        if (wav.channels == 0 || wav.sampleRate == 0 || wav.blockAlign != wav.channels * wav.bytesPerSample()) {
            error = "Inconsistent WAV format header";
            return std::nullopt;
        }
        if (wav.dataSize % wav.blockAlign != 0) {
            error = "WAV data does not contain a whole number of frames";
            return std::nullopt;
        }

        wav.bytes.assign(data.begin(), data.end());
        return wav;
    }

    std::vector<double> wavSamples(const Wav &wav) {
        const std::span<const u8> data(wav.bytes);
        const auto bytes = wav.bytesPerSample();

        std::vector<double> samples(wav.sampleCount());
        for (size_t i = 0; i < samples.size(); i += 1) {
            const auto raw = getLE(data, wav.dataOffset + i * bytes, bytes);
            if (wav.formatTag == FormatIEEEFloat)
                samples[i] = std::bit_cast<float>(raw);
            else if (wav.bitsPerSample == 8)
                samples[i] = raw;
            else
                // Shift the sign bit up to bit 31, then arithmetic-shift back down to sign extend
                samples[i] = i32(raw << (32 - wav.bitsPerSample)) >> (32 - wav.bitsPerSample);
        }

        return samples;
    }

    double wavSampleRange(const Wav &wav) {
        if (wav.formatTag == FormatIEEEFloat)
            return 2.0;

        return std::ldexp(1.0, wav.bitsPerSample) - 1.0;
    }

}
