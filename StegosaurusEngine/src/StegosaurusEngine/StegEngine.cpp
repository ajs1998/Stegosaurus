#include <stegosaurus_pch.h>

#include "StegEngine.h"
#include <StegosaurusEngine\Image\RGBImage.h>

#include <random>

namespace Steg {

    void StegEngine::Encode(Image& image, const std::vector<byte>& data, const EncoderSettings& settings) {

        // TODO Prepend data vector with header information
        std::vector<byte> header();

        // Width of the image
        uint32_t width = image.GetWidth();
        // Height of the image
        uint32_t height = image.GetHeight();
        // Pixel width of the image
        uint32_t bytesPerPixel = image.GetPixelWidth();
        // Number of pixels in the image
        uint32_t pixelCount = width * height;
        // Skip over the alpha channel while encoding
        bool skipAlpha = !(image.HasAlpha() && settings.EncodeInAlpha);

        // Check if data and its depth will fit in the image
        // Note: This check is not 100% accurate. The payload will be prepended with a header containing even more information
        // TODO Make this 100% accurate
        if (8 / settings.DataDepth * data.size() > bytesPerPixel * pixelCount) {
            // Not enough available space in the image for the data and its depth
            // TODO Throw a fit
            return;
        }

        // Create an index vector that holds all possible indices for data to be hidden in
        // An index corresponds to a byte within a pixel and the seed is the first byte of the first channel of the first pixel
        // Index 0 is invalid because the seed for the RNG is stored there
        uint32_t indexCount = pixelCount * bytesPerPixel;

        uint32_t seed = image.GetByte(0);

        // Random Engine generates integers on [0, indexCount - 2]
        std::default_random_engine generator(seed);
        std::uniform_int_distribution<uint32_t> rand(0, indexCount - 2);

        // Fill the index vector
        std::vector<uint32_t> indices(indexCount - 1);
        for (uint32_t i = 0; i < indices.size(); i++) {
            indices[i] = i + 1;
        }

        // Indices are bytes and they are ordered randomly
        // Note that we pull *** values from the RNG in the process
        for (uint32_t i = 0; i < indices.size() - 2; i++) {
            uint32_t j = i + (rand(generator) % (indexCount - 1 - i));
            std::iter_swap(indices.begin() + i, indices.begin() + j);
        }

        /* Hide information in the image */

        const uint16_t pixelMask = GetPixelMask(image.GetBitDepth(), settings.DataDepth);
        const byte partMask = GetPartMask(image.GetBitDepth(), settings.DataDepth);

        // Get a byte of data and insert it into the image
        uint32_t byteIndex, k = 0;
        for (uint32_t i = 0; i < data.size(); i++) {
            byte datum = data[i];

            // Split the byte into parts
            uint32_t partCount = 8 / settings.DataDepth;

            // Get each part and insert it into the image
            for (uint32_t partIndex = 0; partIndex < partCount; partIndex++) {
                byteIndex = indices[k++];

                if (skipAlpha) {
                    // Skip bytes until byteIndex is a color channel
                    while (image.IsAlphaIndex(byteIndex)) {
                        byteIndex = indices[k++];
                    }
                }

                byte shiftAmount = (8 - settings.DataDepth) - (partIndex * settings.DataDepth);
                byte part = (datum >> shiftAmount) & partMask;

                // Combine the data with the image
                byte pixel = image.GetByte(byteIndex) & pixelMask;
                image.SetByte(byteIndex, pixel | part);
            }
        }

    }

    // Ex: bitDepth = 8, dataDepth = 2 => 1111'1111'1111'1100
    // Ex: bitDepth = 8, dataDepth = 4 => 1111'1111'1111'0000
    uint16_t StegEngine::GetPixelMask(uint32_t imageBitDepth, uint32_t dataBitDepth) {
        uint16_t mask = 0xFFFF;
        mask <<= dataBitDepth;
        if (imageBitDepth == 16) {
            return mask;
        }
        else if (imageBitDepth == 8) {
            return mask & 0xFF;
        }
        else {
            // TODO Throw a fit maybe?
            return 0;
        }
    }

    byte StegEngine::GetPartMask(uint32_t imageBitDepth, uint32_t dataBitDepth) {
        byte mask = 0xFF;
        if (imageBitDepth == 16) {
            return mask >> (8 - dataBitDepth);
        }
        else if (imageBitDepth == 8) {
            // TODO Maybe refuse if dataBitDepth == 8 because the entire pixel will be overwritten
            return mask >> (8 - dataBitDepth);
        }
        else {
            // TODO Throw a fit maybe?
            return 0;
        }
    }

}
