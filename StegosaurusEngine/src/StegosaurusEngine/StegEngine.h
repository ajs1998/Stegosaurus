#pragma once

#include <StegosaurusEngine/Core.h>

#include "Image/Image.h"

namespace Steg {

    struct EncoderSettings {

        // Split each data byte into DataDepth bits
        // 1, 2, 4, 8 are valid values. 8 is valid for 16 bit images only
        // TODO Add support for all values on [1, BitDepth] (even though higher values are impractical)
        byte DataDepth = 2;

        // TRUE: Hide data in alpha channel if available
        // FALSE: Leave alpha channel alone (preferred)
        // Note: Only applies to RGBA_X or GRAYA_X PixelModes
        bool EncodeInAlpha = false;

        // TODO Normalize image option

        // TODO Encryption options

    };

    class StegEngine {

    public:

        static void Encode(Image& image, const std::vector<byte>& data, const EncoderSettings& settings);

        // static std::vector<byte> Decode(const Image& image);

    private:

        static uint16_t GetPixelMask(uint32_t imageBitDepth, uint32_t dataBitDepth);

        static byte GetPartMask(uint32_t imageBitDepth, uint32_t dataBitDepth);

    };

}
