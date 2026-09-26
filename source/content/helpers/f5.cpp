#include <content/helpers/f5.hpp>

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <limits>
#include <numbers>
#include <random>
#include <string>
#include <utility>

namespace hex::plugin::stegoooo {

    namespace {

        constexpr size_t BlockSize  = 8;
        constexpr size_t BlockArea  = BlockSize * BlockSize;
        constexpr size_t HeaderBits = 4 + 32;

        constexpr std::array<int, BlockArea> LuminanceTable = {
            16, 11, 10, 16, 24, 40, 51, 61,
            12, 12, 14, 19, 26, 58, 60, 55,
            14, 13, 16, 24, 40, 57, 69, 56,
            14, 17, 22, 29, 51, 87, 80, 62,
            18, 22, 37, 56, 68, 109, 103, 77,
            24, 35, 55, 64, 81, 104, 113, 92,
            49, 64, 78, 87, 103, 121, 120, 101,
            72, 92, 95, 98, 112, 100, 103, 99
        };

        constexpr std::array<int, BlockArea> ChrominanceTable = {
            17, 18, 24, 47, 99, 99, 99, 99,
            18, 21, 26, 66, 99, 99, 99, 99,
            24, 26, 56, 99, 99, 99, 99, 99,
            47, 66, 99, 99, 99, 99, 99, 99,
            99, 99, 99, 99, 99, 99, 99, 99,
            99, 99, 99, 99, 99, 99, 99, 99,
            99, 99, 99, 99, 99, 99, 99, 99,
            99, 99, 99, 99, 99, 99, 99, 99
        };

        struct QuantizedImage {
            Image source;
            u32 blocksWide = 0;
            u32 blocksHigh = 0;
            std::array<std::vector<int>, 3> coefficients;

            [[nodiscard]] size_t blockCount() const {
                return size_t(this->blocksWide) * this->blocksHigh;
            }
        };

        [[nodiscard]] bool validSettings(const F5Settings &settings) {
            return settings.quality >= 1 && settings.quality <= 90 &&
                   (settings.channels == F5Channels::Luma || settings.channels == F5Channels::All);
        }

        [[nodiscard]] u8 clampToByte(double value) {
            return u8(std::clamp(std::lround(value), 0L, 255L));
        }

        [[nodiscard]] std::array<int, BlockArea> makeQuantizationTable(const std::array<int, BlockArea> &base, int quality) {
            std::array<int, BlockArea> table;
            const auto scale = quality < 50 ? 5000 / quality : 200 - 2 * quality;

            for (size_t i = 0; i < table.size(); i += 1)
                table[i] = std::clamp((base[i] * scale + 50) / 100, 1, 255);

            return table;
        }

        [[nodiscard]] const std::array<std::array<double, BlockSize>, BlockSize>& dctBasis() {
            static const auto basis = [] {
                std::array<std::array<double, BlockSize>, BlockSize> result;
                for (size_t frequency = 0; frequency < BlockSize; frequency += 1) {
                    const auto scale = frequency == 0 ? 1.0 / std::sqrt(8.0) : 0.5;
                    for (size_t position = 0; position < BlockSize; position += 1) {
                        result[frequency][position] = scale * std::cos(
                            (2.0 * double(position) + 1.0) * double(frequency) * std::numbers::pi / 16.0
                        );
                    }
                }
                return result;
            }();
            return basis;
        }

        [[nodiscard]] std::array<double, BlockArea> forwardDCT(const std::array<double, BlockArea> &input) {
            const auto &basis = dctBasis();
            std::array<double, BlockArea> temporary = { };
            std::array<double, BlockArea> output = { };

            for (size_t y = 0; y < BlockSize; y += 1) {
                for (size_t u = 0; u < BlockSize; u += 1) {
                    for (size_t x = 0; x < BlockSize; x += 1)
                        temporary[y * BlockSize + u] += input[y * BlockSize + x] * basis[u][x];
                }
            }
            for (size_t v = 0; v < BlockSize; v += 1) {
                for (size_t u = 0; u < BlockSize; u += 1) {
                    for (size_t y = 0; y < BlockSize; y += 1)
                        output[v * BlockSize + u] += basis[v][y] * temporary[y * BlockSize + u];
                }
            }

            return output;
        }

        [[nodiscard]] std::array<double, BlockArea> inverseDCT(const std::array<double, BlockArea> &input) {
            const auto &basis = dctBasis();
            std::array<double, BlockArea> temporary = { };
            std::array<double, BlockArea> output = { };

            for (size_t v = 0; v < BlockSize; v += 1) {
                for (size_t x = 0; x < BlockSize; x += 1) {
                    for (size_t u = 0; u < BlockSize; u += 1)
                        temporary[v * BlockSize + x] += input[v * BlockSize + u] * basis[u][x];
                }
            }
            for (size_t y = 0; y < BlockSize; y += 1) {
                for (size_t x = 0; x < BlockSize; x += 1) {
                    for (size_t v = 0; v < BlockSize; v += 1)
                        output[y * BlockSize + x] += basis[v][y] * temporary[v * BlockSize + x];
                }
            }

            return output;
        }

        using QuantizedBlock = std::array<std::array<int, BlockArea>, 3>;

        [[nodiscard]] QuantizedBlock quantizeBlock(const Image &image, u32 blockX, u32 blockY,
                                                   const std::array<int, BlockArea> &lumaTable,
                                                   const std::array<int, BlockArea> &chromaTable) {
            std::array<std::array<double, BlockArea>, 3> values;
            for (size_t y = 0; y < BlockSize; y += 1) {
                for (size_t x = 0; x < BlockSize; x += 1) {
                    const auto pixel = (size_t(blockY) * BlockSize + y) * image.width + size_t(blockX) * BlockSize + x;
                    const auto r = double(image.rgba[pixel * 4 + 0]);
                    const auto g = double(image.rgba[pixel * 4 + 1]);
                    const auto b = double(image.rgba[pixel * 4 + 2]);
                    const auto offset = y * BlockSize + x;

                    values[0][offset] =  0.299000 * r + 0.587000 * g + 0.114000 * b - 128.0;
                    values[1][offset] = -0.168736 * r - 0.331264 * g + 0.500000 * b;
                    values[2][offset] =  0.500000 * r - 0.418688 * g - 0.081312 * b;
                }
            }

            QuantizedBlock result;
            for (size_t channel = 0; channel < result.size(); channel += 1) {
                const auto dct = forwardDCT(values[channel]);
                const auto &table = channel == 0 ? lumaTable : chromaTable;
                for (size_t i = 0; i < BlockArea; i += 1)
                    result[channel][i] = int(std::lround(dct[i] / double(table[i])));
            }
            return result;
        }

        [[nodiscard]] Image squashImage(const Image &image, u32 margin) {
            Image result = image;
            for (size_t pixel = 0; pixel < result.pixelCount(); pixel += 1) {
                for (size_t channel = 0; channel < 3; channel += 1) {
                    const auto value = double(result.rgba[pixel * 4 + channel]);
                    result.rgba[pixel * 4 + channel] = u8(std::lround(double(margin) + value * double(255 - 2 * margin) / 255.0));
                }
            }
            return result;
        }

        [[nodiscard]] std::optional<QuantizedImage> quantizeImage(const Image &image, int quality, bool squash,
                std::string &error, u32 margin = 16) {
            if (!image.isValid()) {
                error = "Invalid image";
                return std::nullopt;
            }
            if (quality < 1 || quality > 90) {
                error = "Quality must be between 1 and 90";
                return std::nullopt;
            }
            if (squash && margin > 127) {
                error = "Squash margin must be between 0 and 127";
                return std::nullopt;
            }

            QuantizedImage result;
            result.source = squash ? squashImage(image, margin) : image;
            result.blocksWide = image.width / BlockSize;
            result.blocksHigh = image.height / BlockSize;
            if (result.blockCount() == 0) {
                error = "Image must contain at least one full 8 x 8 block";
                return std::nullopt;
            }

            const auto lumaTable = makeQuantizationTable(LuminanceTable, quality);
            const auto chromaTable = makeQuantizationTable(ChrominanceTable, quality);
            for (auto &channel : result.coefficients)
                channel.resize(result.blockCount() * BlockArea);

            for (u32 blockY = 0; blockY < result.blocksHigh; blockY += 1) {
                for (u32 blockX = 0; blockX < result.blocksWide; blockX += 1) {
                    const auto block = size_t(blockY) * result.blocksWide + blockX;
                    const auto values = quantizeBlock(result.source, blockX, blockY, lumaTable, chromaTable);
                    for (size_t channel = 0; channel < result.coefficients.size(); channel += 1) {
                        for (size_t i = 0; i < BlockArea; i += 1)
                            result.coefficients[channel][block * BlockArea + i] = values[channel][i];
                    }
                }
            }

            return result;
        }

        void writeBlock(Image &image, u32 blockX, u32 blockY,
                        const std::array<std::array<double, BlockArea>, 3> &values,
                        std::mt19937 *rng = nullptr) {
            for (size_t y = 0; y < BlockSize; y += 1) {
                for (size_t x = 0; x < BlockSize; x += 1) {
                    const auto offset = y * BlockSize + x;
                    const auto pixel = (size_t(blockY) * BlockSize + y) * image.width + size_t(blockX) * BlockSize + x;
                    const auto yValue = values[0][offset] + 128.0;
                    const auto cb = values[1][offset];
                    const auto cr = values[2][offset];
                    const std::array rgb = {
                        yValue + 1.402000 * cr,
                        yValue - 0.344136 * cb - 0.714136 * cr,
                        yValue + 1.772000 * cb
                    };

                    for (size_t channel = 0; channel < rgb.size(); channel += 1) {
                        if (rng == nullptr) {
                            image.rgba[pixel * 4 + channel] = clampToByte(rgb[channel]);
                        } else {
                            const auto random = double((*rng)()) / (double(std::mt19937::max()) + 1.0);
                            image.rgba[pixel * 4 + channel] = u8(std::clamp(std::floor(rgb[channel] + random), 0.0, 255.0));
                        }
                    }
                }
            }
        }

        [[nodiscard]] bool blockMatches(const Image &image, const QuantizedImage &quantized, size_t block,
                                        const std::array<int, BlockArea> &lumaTable,
                                        const std::array<int, BlockArea> &chromaTable);

        [[nodiscard]] std::array<std::array<double, BlockArea>, 3> projectBlock(
                Image &image, const QuantizedImage &quantized, size_t block,
                const std::array<int, BlockArea> &lumaTable,
                const std::array<int, BlockArea> &chromaTable,
                const std::array<std::array<double, BlockArea>, 3> &initial) {
            std::array<std::array<double, BlockArea>, 3> rgb;
            for (size_t i = 0; i < BlockArea; i += 1) {
                const auto y = initial[0][i] + 128.0;
                rgb[0][i] = y + 1.402000 * initial[2][i];
                rgb[1][i] = y - 0.344136 * initial[1][i] - 0.714136 * initial[2][i];
                rgb[2][i] = y + 1.772000 * initial[1][i];
            }

            const auto blockX = u32(block % quantized.blocksWide);
            const auto blockY = u32(block / quantized.blocksWide);
            std::array<std::array<double, BlockArea>, 3> values;
            for (size_t iteration = 0; iteration < 100; iteration += 1) {
                for (auto &channel : rgb) {
                    for (auto &value : channel)
                        value = std::clamp(value, 0.0, 255.0);
                }

                for (size_t i = 0; i < BlockArea; i += 1) {
                    values[0][i] =  0.299000 * rgb[0][i] + 0.587000 * rgb[1][i] + 0.114000 * rgb[2][i] - 128.0;
                    values[1][i] = -0.168736 * rgb[0][i] - 0.331264 * rgb[1][i] + 0.500000 * rgb[2][i];
                    values[2][i] =  0.500000 * rgb[0][i] - 0.418688 * rgb[1][i] - 0.081312 * rgb[2][i];
                }

                writeBlock(image, blockX, blockY, values);
                if (blockMatches(image, quantized, block, lumaTable, chromaTable))
                    return values;

                for (size_t channel = 0; channel < values.size(); channel += 1) {
                    auto dct = forwardDCT(values[channel]);
                    const auto &table = channel == 0 ? lumaTable : chromaTable;
                    for (size_t i = 1; i < BlockArea; i += 1) {
                        const auto target = double(quantized.coefficients[channel][block * BlockArea + i]);
                        dct[i] = std::clamp(dct[i], (target - 0.4) * table[i], (target + 0.4) * table[i]);
                    }
                    values[channel] = inverseDCT(dct);
                }

                for (size_t i = 0; i < BlockArea; i += 1) {
                    const auto y = values[0][i] + 128.0;
                    rgb[0][i] = y + 1.402000 * values[2][i];
                    rgb[1][i] = y - 0.344136 * values[1][i] - 0.714136 * values[2][i];
                    rgb[2][i] = y + 1.772000 * values[1][i];
                }
            }

            for (auto &channel : rgb) {
                for (auto &value : channel)
                    value = std::clamp(value, 0.0, 255.0);
            }
            for (size_t i = 0; i < BlockArea; i += 1) {
                values[0][i] =  0.299000 * rgb[0][i] + 0.587000 * rgb[1][i] + 0.114000 * rgb[2][i] - 128.0;
                values[1][i] = -0.168736 * rgb[0][i] - 0.331264 * rgb[1][i] + 0.500000 * rgb[2][i];
                values[2][i] =  0.500000 * rgb[0][i] - 0.418688 * rgb[1][i] - 0.081312 * rgb[2][i];
            }
            return values;
        }

        [[nodiscard]] bool blockMatches(const Image &image, const QuantizedImage &quantized, size_t block,
                                        const std::array<int, BlockArea> &lumaTable,
                                        const std::array<int, BlockArea> &chromaTable) {
            const auto blockX = u32(block % quantized.blocksWide);
            const auto blockY = u32(block / quantized.blocksWide);
            const auto actual = quantizeBlock(image, blockX, blockY, lumaTable, chromaTable);
            for (size_t channel = 0; channel < actual.size(); channel += 1) {
                for (size_t i = 1; i < BlockArea; i += 1) {
                    if (actual[channel][i] != quantized.coefficients[channel][block * BlockArea + i])
                        return false;
                }
            }
            return true;
        }

        [[nodiscard]] Image reconstructImage(const QuantizedImage &quantized, int quality, size_t &repairedBlocks) {
            Image result = quantized.source;
            const auto lumaTable = makeQuantizationTable(LuminanceTable, quality);
            const auto chromaTable = makeQuantizationTable(ChrominanceTable, quality);
            repairedBlocks = 0;

            for (u32 blockY = 0; blockY < quantized.blocksHigh; blockY += 1) {
                for (u32 blockX = 0; blockX < quantized.blocksWide; blockX += 1) {
                    const auto block = size_t(blockY) * quantized.blocksWide + blockX;
                    std::array<std::array<double, BlockArea>, 3> values;
                    for (size_t channel = 0; channel < quantized.coefficients.size(); channel += 1) {
                        std::array<double, BlockArea> dequantized;
                        const auto &table = channel == 0 ? lumaTable : chromaTable;
                        for (size_t i = 0; i < BlockArea; i += 1)
                            dequantized[i] = double(quantized.coefficients[channel][block * BlockArea + i] * table[i]);
                        values[channel] = inverseDCT(dequantized);
                    }

                    writeBlock(result, blockX, blockY, values);
                    if (blockMatches(result, quantized, block, lumaTable, chromaTable))
                        continue;

                    values = projectBlock(result, quantized, block, lumaTable, chromaTable, values);
                    writeBlock(result, blockX, blockY, values);
                    if (blockMatches(result, quantized, block, lumaTable, chromaTable)) {
                        repairedBlocks += 1;
                        continue;
                    }

                    bool repaired = false;
                    for (u32 attempt = 0; attempt < 256; attempt += 1) {
                        std::seed_seq seed {
                            u32(block), u32(u64(block) >> 32), attempt, 0xF500F500U
                        };
                        std::mt19937 rng(seed);
                        writeBlock(result, blockX, blockY, values, &rng);
                        if (blockMatches(result, quantized, block, lumaTable, chromaTable)) {
                            repaired = true;
                            repairedBlocks += 1;
                            break;
                        }
                    }
                    if (!repaired)
                        writeBlock(result, blockX, blockY, values);
                }
            }

            return result;
        }

        [[nodiscard]] u64 passwordSeed(const std::string &password) {
            u64 hash = 14695981039346656037ULL;
            for (const auto value : password) {
                hash ^= u8(value);
                hash *= 1099511628211ULL;
            }
            return hash;
        }

        [[nodiscard]] std::vector<int*> makeCarriers(QuantizedImage &image, const F5Settings &settings) {
            std::vector<int*> carriers;
            const auto channelCount = settings.channels == F5Channels::Luma ? size_t(1) : size_t(3);
            carriers.reserve(image.blockCount() * 63 * channelCount);

            for (size_t channel = 0; channel < channelCount; channel += 1) {
                for (size_t block = 0; block < image.blockCount(); block += 1) {
                    for (size_t coefficient = 1; coefficient < BlockArea; coefficient += 1)
                        carriers.push_back(&image.coefficients[channel][block * BlockArea + coefficient]);
                }
            }

            std::mt19937_64 rng(passwordSeed(settings.password));
            for (size_t i = carriers.size(); i > 1; i -= 1)
                std::swap(carriers[i - 1], carriers[size_t(rng() % i)]);

            return carriers;
        }

        [[nodiscard]] std::vector<const int*> makeCarriers(const QuantizedImage &image, const F5Settings &settings) {
            std::vector<const int*> carriers;
            const auto channelCount = settings.channels == F5Channels::Luma ? size_t(1) : size_t(3);
            carriers.reserve(image.blockCount() * 63 * channelCount);

            for (size_t channel = 0; channel < channelCount; channel += 1) {
                for (size_t block = 0; block < image.blockCount(); block += 1) {
                    for (size_t coefficient = 1; coefficient < BlockArea; coefficient += 1)
                        carriers.push_back(&image.coefficients[channel][block * BlockArea + coefficient]);
                }
            }

            std::mt19937_64 rng(passwordSeed(settings.password));
            for (size_t i = carriers.size(); i > 1; i -= 1)
                std::swap(carriers[i - 1], carriers[size_t(rng() % i)]);

            return carriers;
        }

        [[nodiscard]] int coefficientBit(int coefficient) {
            return coefficient > 0 ? coefficient & 1 : 1 - ((-coefficient) & 1);
        }

        void decrementMagnitude(int &coefficient) {
            coefficient += coefficient > 0 ? -1 : 1;
        }

        template<typename Carrier>
        [[nodiscard]] bool nextNonZero(const std::vector<Carrier> &carriers, size_t &cursor, Carrier &carrier) {
            while (cursor < carriers.size()) {
                carrier = carriers[cursor++];
                if (*carrier != 0)
                    return true;
            }
            return false;
        }

        [[nodiscard]] std::vector<u8> makeHeader(u32 k, u32 length) {
            std::vector<u8> bits;
            bits.reserve(HeaderBits);
            for (size_t bit = 0; bit < 4; bit += 1)
                bits.push_back(u8((k >> (3 - bit)) & 1));
            for (size_t byte = 0; byte < 4; byte += 1) {
                const auto value = u8(length >> (byte * 8));
                for (size_t bit = 0; bit < 8; bit += 1)
                    bits.push_back(u8((value >> (7 - bit)) & 1));
            }
            return bits;
        }

        [[nodiscard]] std::vector<u8> makeMessageBits(std::span<const u8> message) {
            std::vector<u8> bits;
            bits.reserve(message.size() * 8);
            for (const auto value : message) {
                for (size_t bit = 0; bit < 8; bit += 1)
                    bits.push_back(u8((value >> (7 - bit)) & 1));
            }
            return bits;
        }

        [[nodiscard]] bool embedPlain(std::vector<int*> &carriers, size_t &cursor, std::span<const u8> bits, F5Stats &stats) {
            for (const auto bit : bits) {
                while (true) {
                    int *coefficient = nullptr;
                    if (!nextNonZero(carriers, cursor, coefficient))
                        return false;
                    if (coefficientBit(*coefficient) == bit)
                        break;

                    decrementMagnitude(*coefficient);
                    stats.changes += 1;
                    if (*coefficient != 0)
                        break;
                    stats.shrinkage += 1;
                }
            }
            return true;
        }

        [[nodiscard]] bool embedMatrix(std::vector<int*> &carriers, size_t &cursor, std::span<const u8> bits, u32 k, F5Stats &stats) {
            const auto groupSize = (size_t(1) << k) - 1;
            std::vector<int*> group;
            group.reserve(groupSize);

            for (size_t bitOffset = 0; bitOffset < bits.size(); bitOffset += k) {
                group.clear();
                while (group.size() < groupSize) {
                    int *coefficient = nullptr;
                    if (!nextNonZero(carriers, cursor, coefficient))
                        return false;
                    group.push_back(coefficient);
                }

                u32 payload = 0;
                for (size_t bit = 0; bit < k; bit += 1) {
                    payload <<= 1;
                    if (bitOffset + bit < bits.size())
                        payload |= bits[bitOffset + bit];
                }

                while (true) {
                    u32 hash = 0;
                    for (size_t i = 0; i < group.size(); i += 1) {
                        if (coefficientBit(*group[i]) != 0)
                            hash ^= u32(i + 1);
                    }

                    const auto change = hash ^ payload;
                    if (change == 0)
                        break;

                    decrementMagnitude(*group[change - 1]);
                    stats.changes += 1;
                    if (*group[change - 1] != 0)
                        break;

                    stats.shrinkage += 1;
                    group.erase(group.begin() + std::ptrdiff_t(change - 1));
                    int *replacement = nullptr;
                    if (!nextNonZero(carriers, cursor, replacement))
                        return false;
                    group.push_back(replacement);
                }
            }
            return true;
        }

        [[nodiscard]] bool readPlain(const std::vector<const int*> &carriers, size_t &cursor, size_t count, std::vector<u8> &bits) {
            bits.clear();
            bits.reserve(count);
            while (bits.size() < count) {
                const int *coefficient = nullptr;
                if (!nextNonZero(carriers, cursor, coefficient))
                    return false;
                bits.push_back(u8(coefficientBit(*coefficient)));
            }
            return true;
        }

        [[nodiscard]] bool readMatrix(const std::vector<const int*> &carriers, size_t &cursor, size_t bitCount, u32 k, std::vector<u8> &bits) {
            const auto groupSize = (size_t(1) << k) - 1;
            bits.clear();
            bits.reserve(bitCount);

            while (bits.size() < bitCount) {
                u32 hash = 0;
                for (size_t i = 0; i < groupSize; i += 1) {
                    const int *coefficient = nullptr;
                    if (!nextNonZero(carriers, cursor, coefficient))
                        return false;
                    if (coefficientBit(*coefficient) != 0)
                        hash ^= u32(i + 1);
                }
                for (size_t bit = 0; bit < k && bits.size() < bitCount; bit += 1)
                    bits.push_back(u8((hash >> (k - 1 - bit)) & 1));
            }
            return true;
        }

        [[nodiscard]] size_t nonZeroCount(const std::vector<const int*> &carriers, size_t start = 0) {
            return size_t(std::count_if(carriers.begin() + std::ptrdiff_t(start), carriers.end(), [](const int *value) {
                return *value != 0;
            }));
        }

        struct CarrierCounts {
            size_t ones = 0;
            size_t large = 0;
        };

        [[nodiscard]] CarrierCounts countCarriers(const std::vector<const int*> &carriers) {
            CarrierCounts result;
            for (const auto coefficient : carriers) {
                const auto magnitude = std::abs(*coefficient);
                if (magnitude == 1)
                    result.ones += 1;
                else if (magnitude > 1)
                    result.large += 1;
            }
            return result;
        }

        [[nodiscard]] double estimatedCapacityBits(const CarrierCounts &counts, u32 k) {
            const auto expected = double(counts.large) + 0.49 * double(counts.ones);
            const auto groupSize = (size_t(1) << k) - 1;
            return std::max(0.0, expected * double(k) / double(groupSize) - double(HeaderBits));
        }

        [[nodiscard]] std::string tooLargeError(size_t requested, size_t capacity) {
            return "Message is too large for this image (" + std::to_string(requested) + "/" + std::to_string(capacity) + " bytes)";
        }

    }

    std::optional<Image> f5Embed(const Image &cover, std::span<const u8> message, const F5Settings &settings, F5Stats &stats, std::string &error) {
        stats = { };
        if (!validSettings(settings)) {
            error = "Quality must be between 1 and 90";
            return std::nullopt;
        }
        if (message.size() > std::numeric_limits<u32>::max()) {
            error = tooLargeError(message.size(), f5Capacity(cover, settings));
            return std::nullopt;
        }

        const auto messageBits = makeMessageBits(message);
        constexpr std::array<u32, 4> SquashMargins = { 16, 24, 32, 48 };
        size_t capacity = 0;
        bool reachedVerification = false;

        for (const auto margin : SquashMargins) {
            auto quantized = quantizeImage(cover, settings.quality, true, error, margin);
            if (!quantized.has_value())
                return std::nullopt;

            const auto carrierCounts = countCarriers(makeCarriers(std::as_const(*quantized), settings));
            F5Stats baseStats;
            baseStats.squashMargin = margin;
            baseStats.usableCoefficients = carrierCounts.ones + carrierCounts.large;
            baseStats.messageBits = message.size() * 8;

            for (u32 candidate = 1; candidate <= 15; candidate += 1) {
                if (double(baseStats.messageBits) <= estimatedCapacityBits(carrierCounts, candidate))
                    baseStats.k = candidate;
            }

            const auto attemptCapacity = size_t(estimatedCapacityBits(carrierCounts, 1)) / 8;
            if (margin == SquashMargins.front())
                capacity = attemptCapacity;
            // A wider margin changes the coefficients, so a failure here only rules out this margin
            if (baseStats.k == 0)
                continue;

            bool embedded = false;
            for (u32 candidate = baseStats.k; candidate >= 1; candidate -= 1) {
                auto attempt = *quantized;
                auto carriers = makeCarriers(attempt, settings);
                auto attemptStats = baseStats;
                attemptStats.k = candidate;

                size_t cursor = 0;
                const auto header = makeHeader(candidate, u32(message.size()));
                if (embedPlain(carriers, cursor, header, attemptStats) &&
                    embedMatrix(carriers, cursor, messageBits, candidate, attemptStats)) {
                    *quantized = std::move(attempt);
                    stats = attemptStats;
                    embedded = true;
                    break;
                }
            }
            if (!embedded)
                continue;

            reachedVerification = true;

            auto result = reconstructImage(*quantized, settings.quality, stats.repairedBlocks);
            std::string verificationError;
            const auto extracted = f5Extract(result, settings, verificationError);
            if (extracted.has_value() && *extracted == std::vector<u8>(message.begin(), message.end()))
                return result;
        }

        error = reachedVerification ? "Embedding could not be verified; try a lower quality" : tooLargeError(message.size(), capacity);
        return std::nullopt;
    }

    std::optional<std::vector<u8>> f5Extract(const Image &stego, const F5Settings &settings, std::string &error) {
        constexpr const char *NoMessageError = "No F5 message found (wrong password, quality or channel setting?)";
        if (!validSettings(settings)) {
            error = "Quality must be between 1 and 90";
            return std::nullopt;
        }

        std::string quantizeError;
        const auto quantized = quantizeImage(stego, settings.quality, false, quantizeError);
        if (!quantized.has_value()) {
            error = quantizeError;
            return std::nullopt;
        }

        const auto carriers = makeCarriers(*quantized, settings);
        size_t cursor = 0;
        std::vector<u8> header;
        if (!readPlain(carriers, cursor, HeaderBits, header)) {
            error = NoMessageError;
            return std::nullopt;
        }

        u32 k = 0;
        for (size_t bit = 0; bit < 4; bit += 1)
            k = (k << 1) | header[bit];
        u32 length = 0;
        for (size_t byte = 0; byte < 4; byte += 1) {
            u8 value = 0;
            for (size_t bit = 0; bit < 8; bit += 1)
                value = u8((value << 1) | header[4 + byte * 8 + bit]);
            length |= u32(value) << (byte * 8);
        }

        // Each group of 2^k - 1 non-zero coefficients carries k bits, so reject impossible lengths before allocating
        const auto remaining = nonZeroCount(carriers, cursor);
        if (k < 1 || k > 15 || (u64(length) * 8 + k - 1) / k * ((u64(1) << k) - 1) > u64(remaining)) {
            error = NoMessageError;
            return std::nullopt;
        }

        std::vector<u8> bits;
        if (!readMatrix(carriers, cursor, size_t(length) * 8, k, bits)) {
            error = NoMessageError;
            return std::nullopt;
        }

        std::vector<u8> message(length);
        for (size_t byte = 0; byte < message.size(); byte += 1) {
            for (size_t bit = 0; bit < 8; bit += 1)
                message[byte] = u8((message[byte] << 1) | bits[byte * 8 + bit]);
        }
        return message;
    }

    std::optional<Image> jpegQuantize(const Image &cover, int quality, std::string &error, u32 margin) {
        auto quantized = quantizeImage(cover, quality, true, error, margin);
        if (!quantized.has_value())
            return std::nullopt;
        size_t repairedBlocks = 0;
        return reconstructImage(*quantized, quality, repairedBlocks);
    }

    size_t f5Capacity(const Image &cover, const F5Settings &settings) {
        if (!validSettings(settings))
            return 0;

        std::string error;
        const auto quantized = quantizeImage(cover, settings.quality, true, error);
        if (!quantized.has_value())
            return 0;
        const auto counts = countCarriers(makeCarriers(std::as_const(*quantized), settings));
        return size_t(estimatedCapacityBits(counts, 1)) / 8;
    }

}
