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

        byte ToByte() const {

            // DataDepth has 4 possible values so it will occupy 2 bits
            byte result = 0;
            if (DataDepth == 1) {
                result |= 0b00;
            }
            else if (DataDepth == 2) {
                result |= 0b01;
            }
            else if (DataDepth == 4) {
                result |= 0b10;
            }
            else if (DataDepth == 8) {
                result |= 0b11;
            }
            else {
                // TODO Throw a fit probably
                return 0;
            }
            result <<= 1;

            // Each bool has 2 possible values so they will occupy 1 bit each
            result |= (byte)EncodeInAlpha;

            // TODO Add more bool flags here as needed

            return result;
        }

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
