#include <content/helpers/color_space.hpp>

#include <algorithm>
#include <cmath>

namespace hex::plugin::stegoooo {

    namespace {

        u8 clampToByte(float value) {
            return u8(std::clamp(std::lround(value), 0L, 255L));
        }

        /// Full-range BT.601, the inverse of the transform in toYCbCr()
        void yCbCrToRGB(float y, float cb, float cr, u8 *out) {
            out[0] = clampToByte(y + 1.402F    * (cr - 128.0F));
            out[1] = clampToByte(y - 0.344136F * (cb - 128.0F) - 0.714136F * (cr - 128.0F));
            out[2] = clampToByte(y + 1.772F    * (cb - 128.0F));
            out[3] = 0xFF;
        }

    }

    const std::vector<u8>& YCbCrPlanes::getChannel(YCbCrChannel channel) const {
        switch (channel) {
            case YCbCrChannel::Cb: return this->cb;
            case YCbCrChannel::Cr: return this->cr;
            case YCbCrChannel::Y:
            default:               return this->y;
        }
    }

    YCbCrPlanes toYCbCr(const Image &image) {
        YCbCrPlanes planes;

        if (!image.isValid())
            return planes;

        planes.width  = image.width;
        planes.height = image.height;

        const auto pixelCount = image.pixelCount();
        planes.y.resize(pixelCount);
        planes.cb.resize(pixelCount);
        planes.cr.resize(pixelCount);

        for (size_t i = 0; i < pixelCount; i += 1) {
            const auto r = float(image.rgba[i * 4 + 0]);
            const auto g = float(image.rgba[i * 4 + 1]);
            const auto b = float(image.rgba[i * 4 + 2]);

            planes.y[i]  = clampToByte( 0.299000F * r + 0.587000F * g + 0.114000F * b);
            planes.cb[i] = clampToByte(-0.168736F * r - 0.331264F * g + 0.500000F * b + 128.0F);
            planes.cr[i] = clampToByte( 0.500000F * r - 0.418688F * g - 0.081312F * b + 128.0F);
        }

        return planes;
    }

    std::vector<u8> renderChannel(const YCbCrPlanes &planes, YCbCrChannel channel, ChannelPreview preview) {
        std::vector<u8> rgba;

        if (!planes.isValid())
            return rgba;

        const auto &values = planes.getChannel(channel);
        rgba.resize(planes.pixelCount() * 4);

        for (size_t i = 0; i < values.size(); i += 1) {
            const auto value = values[i];
            auto *pixel = &rgba[i * 4];

            if (preview == ChannelPreview::Grayscale) {
                pixel[0] = value;
                pixel[1] = value;
                pixel[2] = value;
                pixel[3] = 0xFF;
                continue;
            }

            switch (channel) {
                case YCbCrChannel::Y:  yCbCrToRGB(float(value), 128.0F, 128.0F, pixel); break;
                case YCbCrChannel::Cb: yCbCrToRGB(128.0F, float(value), 128.0F, pixel); break;
                case YCbCrChannel::Cr: yCbCrToRGB(128.0F, 128.0F, float(value), pixel); break;
            }
        }

        return rgba;
    }

    std::vector<u8> renderRGBChannel(const Image &image, RGBChannel channel, ChannelPreview preview) {
        std::vector<u8> rgba;

        if (!image.isValid())
            return rgba;

        const auto channelIndex = size_t(channel);
        rgba.resize(image.pixelCount() * 4);

        for (size_t i = 0; i < image.pixelCount(); i += 1) {
            const auto value = image.rgba[i * 4 + channelIndex];
            auto *pixel = &rgba[i * 4];

            pixel[0] = preview == ChannelPreview::Grayscale || channel == RGBChannel::R ? value : 0;
            pixel[1] = preview == ChannelPreview::Grayscale || channel == RGBChannel::G ? value : 0;
            pixel[2] = preview == ChannelPreview::Grayscale || channel == RGBChannel::B ? value : 0;
            pixel[3] = 0xFF;
        }

        return rgba;
    }

    std::vector<u8> toLuma(const Image &image) {
        std::vector<u8> luma;

        if (!image.isValid())
            return luma;

        luma.resize(image.pixelCount());
        for (size_t i = 0; i < image.pixelCount(); i += 1) {
            const auto r = float(image.rgba[i * 4 + 0]);
            const auto g = float(image.rgba[i * 4 + 1]);
            const auto b = float(image.rgba[i * 4 + 2]);
            luma[i] = clampToByte(0.299000F * r + 0.587000F * g + 0.114000F * b);
        }

        return luma;
    }

}
