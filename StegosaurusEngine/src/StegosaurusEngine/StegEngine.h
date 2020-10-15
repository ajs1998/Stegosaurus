#pragma once

#include <StegosaurusEngine/Core.h>

#include "RNG.h"
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

        // TRUE: Encrypt data before encoding
        // FALSE: Leave payload alone
        // Note: Encrypting may increase the payload size slightly
        bool EncryptPayload = false;
        std::vector<byte> EncryptionKey;

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
            result <<= 1;

            result |= (byte)EncryptPayload;

            // TODO Add more bool flags here as needed

            return result;

        }

        static EncoderSettings FromByte(byte settingsByte) {

            EncoderSettings settings;

            settings.EncryptPayload = settingsByte & 0x01;
            settingsByte >>= 1;

            settings.EncodeInAlpha = settingsByte & 0x01;
            settingsByte >>= 1;

            byte depth = settingsByte & 0x11;
            if (depth == 0x00) {
                settings.DataDepth = 1;
            }
            else if (depth == 0x01) {
                settings.DataDepth = 2;
            }
            else if (depth == 0x10) {
                settings.DataDepth = 4;
            }
            else if (depth == 0x11) {
                settings.DataDepth = 8;
            }

            return settings;

        }

    };

    class StegEngine {

    public:

        static void Encode(Image& image, const std::vector<byte>& data, const EncoderSettings& settings);

        static std::vector<byte> Decode(const Image& image, const std::vector<byte> key);

    private:

        static uint16_t GetPixelMask(uint32_t imageBitDepth, uint32_t dataBitDepth);

        static byte GetPartMask(uint32_t imageBitDepth, uint32_t dataBitDepth);

        static std::vector<uint32_t> GenerateIndices(uint32_t seed, RNG& rng);

    };

}
