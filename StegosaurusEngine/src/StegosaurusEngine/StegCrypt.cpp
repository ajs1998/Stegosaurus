#include "stegosaurus_pch.h"
#include "StegCrypt.h"

#include "aes.hpp"
#include "aes.hpp"

#include <ctime>

namespace Steg {

    // These methods will handle the IV in the background
    std::vector<byte> StegCrypt::Encrypt(const std::vector<byte>& key, const std::vector<byte> data) {

        // IV is BLOCK_SIZE bytes long
        std::vector<byte> iv = GetIV();

        // TODO Use a KDF instead (PBKDF2)
        std::vector<byte> paddedKey = AddPadding(key);

        // Data is padded to nearest 16 bytes
        std::vector<byte> dataBuffer = AddPadding(data);

        byte* ivBytes = &iv[0];
        byte* keyBytes = &paddedKey[0];
        byte* dataBytes = &dataBuffer[0];

        AES_ctx context;
        AES_init_ctx_iv(&context, keyBytes, ivBytes);
        AES_CBC_encrypt_buffer(&context, dataBytes, dataBuffer.size());

        // Prepend IV to the data buffer
        dataBuffer.insert(dataBuffer.begin(), iv.begin(), iv.end());

        return dataBuffer;

    }

    std::vector<byte> StegCrypt::Decrypt(const std::vector<byte>& key, const std::vector<byte> data) {

        // IV is BLOCK_SIZE bytes long
        std::vector<byte> iv(data.begin(), data.begin() + BLOCK_SIZE);

        // TODO Use a KDF instead (PBKDF2)
        std::vector<byte> paddedKey = AddPadding(key);

        // Clip off the IV from the front
        std::vector<byte> dataBuffer(data.begin() + BLOCK_SIZE, data.end());

        byte* ivBytes = &iv[0];
        byte* keyBytes = &paddedKey[0];
        byte* dataBytes = &dataBuffer[0];

        AES_ctx context;
        AES_init_ctx_iv(&context, keyBytes, ivBytes);
        AES_CBC_decrypt_buffer(&context, dataBytes, dataBuffer.size());

        return RemovePadding(dataBuffer);

    }

    std::vector<byte> StegCrypt::GetIV() {
        std::vector<byte> iv(IV_LENGTH);
        std::time_t sec = time(NULL);
        int k = 0;
        for (int i = 0; i < 2; i++) {
            iv[k++] = ((sec >> 56) & 0xFF);
            iv[k++] = ((sec >> 40) & 0xFF);
            iv[k++] = ((sec >> 32) & 0xFF);
            iv[k++] = ((sec >> 48) & 0xFF);
            iv[k++] = ((sec >> 24) & 0xFF);
            iv[k++] = ((sec >> 16) & 0xFF);
            iv[k++] = ((sec >> 8) & 0xFF);
            iv[k++] = (sec & 0xFF);
        }
        return iv;
    }

    // PKCS7 Padding
    std::vector<byte> StegCrypt::AddPadding(const std::vector<byte> data) {
        std::vector<byte> paddedData(data);
        byte padAmount = BLOCK_SIZE - (data.size() % BLOCK_SIZE);
        for (byte i = 0; i < padAmount; i++) {
            paddedData.push_back(padAmount);
        }
        return paddedData;
    }

    std::vector<byte> StegCrypt::RemovePadding(const std::vector<byte> data) {
        std::vector<byte> unpaddedData(data);
        uint32_t padAmount = data[data.size() - 1];
        for (byte i = 0; i < padAmount; i++) {
            unpaddedData.pop_back();
        }
        return unpaddedData;
    }

}
