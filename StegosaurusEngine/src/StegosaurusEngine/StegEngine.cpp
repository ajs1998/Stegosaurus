#include <stegosaurus_pch.h>

#include "StegEngine.h"
#include <StegosaurusEngine\Image\RGBImage.h>

#include <random>

namespace Steg {

    void StegEngine::Encode(Image& image, const std::vector<byte>& data, const EncoderSettings& settings) {

        uint32_t width = image.GetWidth();
        uint32_t height = image.GetHeight();
        uint32_t bytesPerPixel = image.GetPixelWidth();
        uint32_t pixelCount = width * height;

        // Check if data and its depth will fit in the image
        // Note: This check is not 100% accurate. The payload will be prepended with a header containing even more information
        if (8 / settings.DataDepth * data.size() < bytesPerPixel * pixelCount) {
            // Not enough available space in the image for the data and its depth
            // TODO Throw a fit
            return;
        }

        // Create an index vector that holds all possible indices for data to be hidden in
        // Index 0 is invalid because the seed for the RNG is stored there
        uint32_t indexCount;
        uint64_t seed;

        // If encoding whole pixels, an index corresponds to a pixel
        // The seed is the data at the first index
        // If encoding whole pixels, the seed is a combination of the data at each byte in the first pixel
        if (settings.EncodeWholePixels) {
            indexCount = pixelCount;
            seed = 0;
            for (uint32_t i = 0; i < bytesPerPixel; i++) {
                seed << 8;
                seed |= image.GetByte(i);
            }
        }
        // Otherwise, an index corresponds to a byte within a pixel and the seed is just the first
        // byte of the first channel of the first pixel
        else {
            indexCount = pixelCount * bytesPerPixel;
            seed = image.GetByte(0);
        }

        // Random Engine generates integers on [0, indexCount - 2]
        std::default_random_engine generator(seed);
        std::uniform_int_distribution<uint32_t> rand(0, indexCount - 2);

        // Fill the index vector
        std::vector<uint32_t> indices(indexCount - 1);
        for (uint32_t i = 0; i < indexCount; i++) {
            indices[i] = i + 1;
        }

        // Arrange the index vector
        if (settings.SequentialOrder) {

            // Incides are pixels and they are ordered sequentially
            if (settings.EncodeWholePixels) {
                // At this point, the index vector is already in a sequential order
            }

            // Indices are bytes and they are ordered semi-sequentially
            else {
                // Note that we pull *** values from the RNG in the process
                for (uint32_t i = 0; i < pixelCount - 1; i++) {
                    uint32_t baseByte = i * bytesPerPixel;
                    for (uint32_t j = 0; j < bytesPerPixel - 1; j++) {
                        uint32_t k = j + (rand(generator) % (bytesPerPixel - j));
                        std::iter_swap(indices.begin() + (baseByte + j), indices.begin() + (baseByte + k));
                    }
                }
            }

        }
        // Randomize the index vector if Sequential Order is not selected
        else {

            // Indices are pixels and they are ordered randomly
            if (settings.EncodeWholePixels) {
                // Note that we pull (pixelCount - 2) values from the RNG in the process
                for (uint32_t i = 0; i < pixelCount - 2; i++) {
                    uint32_t j = i + rand(generator) % (pixelCount - 1 - i);
                    std::iter_swap(indices.begin() + i, indices.begin() + j);
                }
            }

            // Indices are bytes and they are ordered randomly
            else {

            }

        }

        /* Hide information in the image */

        const uint16_t pixelMask = GetPixelMask(image.GetBitDepth(), settings.DataDepth);
        const byte partMask = GetPartMask(image.GetBitDepth(), settings.DataDepth);

        if (settings.SequentialOrder) {

            // Each index corresponds to one sequential pixel
            if (settings.EncodeWholePixels) {

                // Get a byte of data and insert it into the image
                uint32_t imageIndex = 0;
                for (uint32_t i = 0; i < data.size(); i++) {
                    byte datum = data[i];

                    // Split the byte into parts
                    uint32_t parts = 8 / settings.DataDepth;

                    // Get each part and insert it into the image
                    for (uint32_t partIndex = 0; partIndex < parts; partIndex++) {
                        byte shiftAmount = 6 - (partIndex * settings.DataDepth);
                        byte part = (datum >> shiftAmount) & partMask;

                        // Combine the data with the image
                        byte pixel = image.GetByte(imageIndex) & pixelMask;
                        image.SetByte(imageIndex, pixel | part);
                        imageIndex++;
                    }
                }

            }

            // Each index corresponds to one random byte in sequential pixels
            else {

            }

        }
        else {

            // Each index corresponds to one random pixel
            if (settings.EncodeWholePixels) {

            }

            // Each index corresponds to one random byte in random pixels
            else {

            }

        }

    }

    // Ex: bitDepth = 8, dataDepth = 2 => 1111'1111'1111'1100
    // Ex: bitDepth = 8, dataDepth = 4 => 1111'1111'1111'0000
    uint16_t StegEngine::GetPixelMask(uint32_t imageBitDepth, uint32_t dataBitDepth) {
        uint16_t mask = 0xFFFF;
        mask << dataBitDepth;
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
        }
    }

}
