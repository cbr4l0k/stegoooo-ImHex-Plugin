#include <content/helpers/stego.hpp>

#include <content/helpers/color_space.hpp>
#include <content/helpers/todo.hpp>

#include <algorithm>
#include <cmath>
#include <limits>

namespace hex::plugin::stegoooo {

    namespace {

        // Shared LSB core. A cover is just `carrierCount` bytes reachable through
        // `carrier(i)`; bit 0 of each one stores a payload bit. Payload layout:
        // u32 little-endian message length, then the message, each byte MSB first.

        size_t payloadCapacity(size_t carrierCount) {
            const size_t bytes = carrierCount / 8;
            return bytes >= 4 ? bytes - 4 : 0;
        }

        bool embedPayload(size_t carrierCount, auto &&carrier, std::span<const u8> message, std::string &error) {
            if (carrierCount < 32) {
                error = "Cover is too small to contain an LSB payload";
                return false;
            }
            if (message.size() > payloadCapacity(carrierCount) || message.size() > std::numeric_limits<u32>::max()) {
                error = "Message is too large for this cover";
                return false;
            }

            std::vector<u8> payload;

            // Place for a header to store the size of the message
            payload.reserve(message.size() + 4);
            const auto length = u32(message.size());
            // little endian size of the payload
            for (size_t i = 0; i < 4; i += 1)
                payload.push_back(u8(length >> (i * 8)));
            payload.insert(payload.end(), message.begin(), message.end());

            for (size_t bitIndex = 0; bitIndex < payload.size() * 8; bitIndex += 1) {
                const auto byte = payload[bitIndex / 8];
                const auto bit = (byte >> (7 - bitIndex % 8)) & 1;
                u8 &value = carrier(bitIndex);
                // 0xFE => 11111110
                // value & 0xFE =>xxxxxxx0
                // value & 0xFE|bit => xxxxxxx{bit}
                value = u8((value & 0xFE) | bit);
            }

            return true;
        }

        std::optional<std::vector<u8>> extractPayload(size_t carrierCount, auto &&carrier, std::string &error) {
            if (carrierCount < 32) {
                error = "Cover is too small to contain an LSB payload";
                return std::nullopt;
            }

            const auto readByte = [&carrier](size_t byteIndex) {
                u8 value = 0;
                for (size_t bit = 0; bit < 8; bit += 1)
                    value = u8((value << 1) | (carrier(byteIndex * 8 + bit) & 1));
                return value;
            };

            u32 length = 0;
            for (size_t i = 0; i < 4; i += 1)
                length |= u32(readByte(i)) << (i * 8);

            if (size_t(length) > payloadCapacity(carrierCount)) {
                error = "Stored message length exceeds the cover capacity";
                return std::nullopt;
            }

            std::vector<u8> message(length);
            for (size_t i = 0; i < message.size(); i += 1)
                message[i] = readByte(i + 4);

            return message;
        }

        // If the image is RGBA, the alpha is ignored... just working on the color channels
        size_t imageCarrierIndex(size_t i) {
            return (i / 3) * 4 + i % 3;
        }

        // Bit 0 of every sample lives in its first (lowest) byte, since WAV is little endian
        size_t wavCarrierIndex(const Wav &wav, size_t i) {
            return wav.dataOffset + i * wav.bytesPerSample();
        }

        bool checkWavCover(const Wav &wav, std::string &error) {
            if (wav.formatTag != 1) {
                error = "LSB on WAV needs integer PCM samples (float WAV is not supported)";
                return false;
            }
            return true;
        }

        // SSIM of one window of `count` values read through `original(i)` / `modified(i)`;
        // `range` is the dynamic range of the signal (255 for 8-bit pixels)
        double windowSSIM(size_t count, auto &&original, auto &&modified, double range) {
            const double C1 = (0.01 * range) * (0.01 * range);
            const double C2 = (0.03 * range) * (0.03 * range);

            double originalMean = 0.0, modifiedMean = 0.0;
            for (size_t i = 0; i < count; i += 1) {
                originalMean += original(i);
                modifiedMean += modified(i);
            }
            originalMean /= double(count);
            modifiedMean /= double(count);

            double originalVariance = 0.0, modifiedVariance = 0.0, covariance = 0.0;
            for (size_t i = 0; i < count; i += 1) {
                const auto originalDelta = original(i) - originalMean;
                const auto modifiedDelta = modified(i) - modifiedMean;
                originalVariance += originalDelta * originalDelta;
                modifiedVariance += modifiedDelta * modifiedDelta;
                covariance += originalDelta * modifiedDelta;
            }
            originalVariance /= double(count);
            modifiedVariance /= double(count);
            covariance /= double(count);

            return ((2.0 * originalMean * modifiedMean + C1) * (2.0 * covariance + C2)) /
                   ((originalMean * originalMean + modifiedMean * modifiedMean + C1) *
                    (originalVariance + modifiedVariance + C2));
        }

    }

    size_t lsbCapacity(const Image &image) {
        if (!image.isValid())
            return 0;

        // One for each of the RGB channels
        return payloadCapacity(image.pixelCount() * 3);
    }

    std::optional<Image> lsbEmbed(const Image &image, std::span<const u8> message, std::string &error) {
        if (!image.isValid()) {
            error = "Invalid image";
            return std::nullopt;
        }

        Image result = image;
        const auto carrier = [&result](size_t i) -> u8 & { return result.rgba[imageCarrierIndex(i)]; };
        if (!embedPayload(image.pixelCount() * 3, carrier, message, error))
            return std::nullopt;

        return result;
    }

    std::optional<std::vector<u8>> lsbExtract(const Image &image, std::string &error) {
        if (!image.isValid()) {
            error = "The image is not valid. Be sure to use a valid format";
            return std::nullopt;
        }

        const auto carrier = [&image](size_t i) { return image.rgba[imageCarrierIndex(i)]; };
        return extractPayload(image.pixelCount() * 3, carrier, error);
    }

    size_t lsbWavCapacity(const Wav &wav) {
        if (wav.formatTag != 1)
            return 0;

        return payloadCapacity(wav.sampleCount());
    }

    std::optional<std::vector<u8>> lsbWavEmbed(const Wav &wav, std::span<const u8> message, std::string &error) {
        if (!checkWavCover(wav, error))
            return std::nullopt;

        // Edit samples in place: header, other chunks and padding stay byte-for-byte identical
        auto result = wav.bytes;
        const auto carrier = [&](size_t i) -> u8 & { return result[wavCarrierIndex(wav, i)]; };
        if (!embedPayload(wav.sampleCount(), carrier, message, error))
            return std::nullopt;

        return result;
    }

    std::optional<std::vector<u8>> lsbWavExtract(const Wav &wav, std::string &error) {
        if (!checkWavCover(wav, error))
            return std::nullopt;

        const auto carrier = [&wav](size_t i) { return wav.bytes[wavCarrierIndex(wav, i)]; };
        return extractPayload(wav.sampleCount(), carrier, error);
    }

    double mse(const Image &original, const Image &modified) {
        double squaredError = 0.0;
        for (size_t pixel = 0; pixel < original.pixelCount(); pixel += 1) {
            for (size_t channel = 0; channel < 3; channel += 1) {
                const auto difference = double(original.rgba[pixel * 4 + channel]) -double(modified.rgba[pixel * 4 + channel]);
                squaredError += double(difference) * double(difference);
            }
        }

        return squaredError / double(original.pixelCount() * 3);
    }

    double psnr(const Image &original, const Image &modified) {
        const auto error = mse(original, modified);
        if (error == 0.0)
            return std::numeric_limits<double>::infinity();

        return 10.0 * std::log10(255.0 * 255.0 / error);
    }

    double ssim(const Image &original, const Image &modified) {
        const auto originalLuma = toLuma(original);
        const auto modifiedLuma = toLuma(modified);
        const auto windowWidth  = std::min<u32>(8, original.width);
        const auto windowHeight = std::min<u32>(8, original.height);

        double total = 0.0;
        size_t windowCount = 0;
        for (u32 y = 0; y + windowHeight <= original.height; y += 4) {
            for (u32 x = 0; x + windowWidth <= original.width; x += 4) {
                const auto index = [&](size_t i) { return size_t(y + i / windowWidth) * original.width + x + i % windowWidth; };
                total += windowSSIM(size_t(windowWidth) * windowHeight,
                                    [&](size_t i) { return double(originalLuma[index(i)]); },
                                    [&](size_t i) { return double(modifiedLuma[index(i)]); },
                                    255.0);
                windowCount += 1;
            }
        }

        return total / double(windowCount);
    }

    double mse(const Wav &original, const Wav &modified) {
        const auto originalSamples = wavSamples(original);
        const auto modifiedSamples = wavSamples(modified);

        double squaredError = 0.0;
        for (size_t i = 0; i < originalSamples.size(); i += 1) {
            const auto difference = originalSamples[i] - modifiedSamples[i];
            squaredError += difference * difference;
        }

        return squaredError / double(originalSamples.size());
    }

    double psnr(const Wav &original, const Wav &modified) {
        const auto error = mse(original, modified);
        if (error == 0.0)
            return std::numeric_limits<double>::infinity();

        const auto range = wavSampleRange(original);
        return 10.0 * std::log10(range * range / error);
    }

    // 1D analogue of the image SSIM: 64-sample windows with 50% overlap, per channel
    double ssim(const Wav &original, const Wav &modified) {
        const auto originalSamples = wavSamples(original);
        const auto modifiedSamples = wavSamples(modified);
        const auto channels     = size_t(original.channels);
        const auto frames       = original.frameCount();
        const auto windowLength = std::min<size_t>(64, frames);
        const auto range        = wavSampleRange(original);

        double total = 0.0;
        size_t windowCount = 0;
        for (size_t channel = 0; channel < channels; channel += 1) {
            for (size_t start = 0; start + windowLength <= frames; start += std::max<size_t>(1, windowLength / 2)) {
                const auto index = [&](size_t i) { return (start + i) * channels + channel; };
                total += windowSSIM(windowLength,
                                    [&](size_t i) { return originalSamples[index(i)]; },
                                    [&](size_t i) { return modifiedSamples[index(i)]; },
                                    range);
                windowCount += 1;
            }
        }

        return total / double(windowCount);
    }

}
