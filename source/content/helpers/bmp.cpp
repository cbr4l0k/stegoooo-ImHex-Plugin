#include <content/helpers/bmp.hpp>
#include <content/helpers/todo.hpp>

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

        // HINT(T4): a BMP row is 3 bytes per pixel, but every row must be padded up to a multiple of 4 bytes
        STEGO_TODO("T4", "compute the padded size of one pixel row in bytes");
        const size_t rowSize = 0;
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

        // HINT(R4): open the output in an image viewer. Is it the right way up? What does a positive biHeight mean?
        for (u32 y = 0; y < image.height; y += 1) {
            const auto rowStart = bmp.size();
            for (u32 x = 0; x < image.width; x += 1) {
                const auto offset = (size_t(y) * image.width + x) * 4;
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
