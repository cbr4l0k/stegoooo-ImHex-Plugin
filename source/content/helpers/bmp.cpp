#include <content/helpers/bmp.hpp>
#include <content/helpers/todo.hpp>
#include <cstddef>

namespace hex::plugin::stegoooo {

    namespace {

        void putLE(std::vector<u8> &data, u32 value, size_t bytes) {
            for (size_t i = 0; i < bytes; i += 1)
                data.push_back(u8(value >> (i * 8)));
        }

    }

    std::vector<u8> encodeBMP(const Image &image) {
        std::vector<u8> bmp;

        if (!image.isValid())
            return bmp;

        const size_t rowSize = (size_t(image.width) * 3 + 3) / 4 * 4;
        const auto dataSize = rowSize * size_t(image.height);
        const auto fileSize = size_t(54) + dataSize;

        bmp.reserve(fileSize);
        bmp.push_back('B');
        bmp.push_back('M');
        putLE(bmp, u32(fileSize), 4);
        putLE(bmp, 0, 4);
        putLE(bmp, 54, 4);

        putLE(bmp, 40, 4);
        putLE(bmp, image.width, 4);
        putLE(bmp, image.height, 4);
        putLE(bmp, 1, 2);
        putLE(bmp, 24, 2);
        putLE(bmp, 0, 4);
        putLE(bmp, u32(dataSize), 4);
        putLE(bmp, 0, 4);
        putLE(bmp, 0, 4);
        putLE(bmp, 0, 4);
        putLE(bmp, 0, 4);

        for (u32 y = image.height; y > 0; y -= 1) {
            const auto rowStart = bmp.size();
            for (u32 x = 0; x < image.width; x += 1) {
                const auto offset = (size_t(y - 1) * image.width + x) * 4;
                bmp.push_back(image.rgba[offset + 2]);
                bmp.push_back(image.rgba[offset + 1]);
                bmp.push_back(image.rgba[offset + 0]);
            }
            while (bmp.size() - rowStart < rowSize)
                bmp.push_back(0);
        }

        return bmp;
    }

}
