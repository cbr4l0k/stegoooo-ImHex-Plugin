#include <content/helpers/stego.hpp>

#include <content/helpers/color_space.hpp>
#include <content/helpers/todo.hpp>

#include <algorithm>
#include <cmath>
#include <limits>

namespace hex::plugin::stegoooo {

    size_t lsbCapacity(const Image &image) {
        if (!image.isValid())
            return 0;

        // One for each of the RGB channels
        const size_t bytes = image.pixelCount() * 3 / 8;
        return bytes >= 4 ? bytes - 4 : 0;
    }

    std::optional<Image> lsbEmbed(const Image &image, std::span<const u8> message, std::string &error) {
        if (!image.isValid()) {
            error = "Invalid image";
            return std::nullopt;
        }

        const auto capacity = lsbCapacity(image);
        if (message.size() > capacity || message.size() > std::numeric_limits<u32>::max()) {
            error = "Message is too large for this image";
            return std::nullopt;
        }

        std::vector<u8> payload;

        // Place for a header to store the size of the message
        payload.reserve(message.size() + 4);
        const auto length = u32(message.size());
        // little endian size of the payload
        for (size_t i = 0; i < 4; i += 1)
            payload.push_back(u8(length >> (i * 8)));
        payload.insert(payload.end(), message.begin(), message.end());

        Image result = image;
        size_t bitIndex = 0;
        for (size_t pixel = 0; bitIndex < payload.size() * 8; pixel += 1) {
          // If the image is RGBa, it ignores the alpha... just working on the
          // color channels
          for (size_t channel = 0; channel < 3 && bitIndex < payload.size() * 8;
               channel += 1) {
            const auto byte = payload[bitIndex / 8];
            const auto bit = (byte >> (7 - bitIndex % 8)) & 1;
            auto &value = result.rgba[pixel * 4 + channel];
            // 0xFE => 11111110
            // value & 0xFE =>xxxxxxx0
            // value & 0xFE|bit => xxxxxxx{bit}
            value = u8((value & 0xFE) | bit);
            bitIndex += 1;
          }
        }

        return result;
    }

    std::optional<std::vector<u8>> lsbExtract(const Image &image, std::string &error) {
        if (!image.isValid()) {
            error = "The image is not valid. Be sure to use a valid format";
            return std::nullopt;
        }
        if (image.pixelCount() * 3 < 32) {
            error = "Image is too small to contain an LSB payload";
            return std::nullopt;
        }

        const auto readByte = [&image](size_t byteIndex) {
            u8 value = 0;
            for (size_t bit = 0; bit < 8; bit += 1) {
                const auto bitIndex = byteIndex * 8 + bit;
                const auto pixel    = bitIndex / 3;
                const auto channel  = bitIndex % 3;
                value = u8((value << 1) | (image.rgba[pixel * 4 + channel] & 1));
            }
            return value;
        };

        u32 length = 0;
        for (size_t i = 0; i < 4; i += 1)
            length |= u32(readByte(i)) << (i * 8);

        if (size_t(length) > lsbCapacity(image)) {
            error = "Stored message length exceeds the image capacity";
            return std::nullopt;
        }

        std::vector<u8> message(length);
        for (size_t i = 0; i < message.size(); i += 1)
            message[i] = readByte(i + 4);

        return message;
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
        constexpr double C1 = (0.01 * 255.0) * (0.01 * 255.0);
        constexpr double C2 = (0.03 * 255.0) * (0.03 * 255.0);

        double total = 0.0;
        size_t windowCount = 0;
        for (u32 y = 0; y + windowHeight <= original.height; y += 4) {
            for (u32 x = 0; x + windowWidth <= original.width; x += 4) {
                const auto count = double(windowWidth) * double(windowHeight);
                double originalMean = 0.0, modifiedMean = 0.0;

                for (u32 wy = 0; wy < windowHeight; wy += 1) {
                    for (u32 wx = 0; wx < windowWidth; wx += 1) {
                        const auto index = size_t(y + wy) * original.width + x + wx;
                        originalMean += originalLuma[index];
                        modifiedMean += modifiedLuma[index];
                    }
                }
                originalMean /= count;
                modifiedMean /= count;

                double originalVariance = 0.0, modifiedVariance = 0.0, covariance = 0.0;
                for (u32 wy = 0; wy < windowHeight; wy += 1) {
                    for (u32 wx = 0; wx < windowWidth; wx += 1) {
                        const auto index = size_t(y + wy) * original.width + x + wx;
                        const auto originalDelta = double(originalLuma[index]) - originalMean;
                        const auto modifiedDelta = double(modifiedLuma[index]) - modifiedMean;
                        originalVariance += originalDelta * originalDelta;
                        modifiedVariance += modifiedDelta * modifiedDelta;
                        covariance += originalDelta * modifiedDelta;
                    }
                }
                originalVariance /= count;
                modifiedVariance /= count;
                covariance /= count;

                total += ((2.0 * originalMean * modifiedMean + C1) * (2.0 * covariance + C2)) /
                         ((originalMean * originalMean + modifiedMean * modifiedMean + C1) *
                          (originalVariance + modifiedVariance + C2));
                windowCount += 1;
            }
        }

        return total / double(windowCount);
    }

}
