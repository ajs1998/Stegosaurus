#include "stegosaurus_pch.h"
#include "StegCrypt.h"

#include "aes.hpp"
#include "include/argon2.h"

namespace Steg {

    // These methods will handle the IV in the background
    std::vector<byte> StegCrypt::Encrypt(const std::vector<byte>& pass, const std::vector<byte> data) {

        // Create a random number generator with seed 0 for the IV
        RNG rng(0);

        // TODO Use a KDF instead (PBKDF2)
        std::vector<byte> key = DeriveKey(pass, rng);

        // IV is BLOCK_SIZE bytes long
        std::vector<byte> iv = GetIV(rng);

        // Data is padded to nearest 16 bytes
        std::vector<byte> dataBuffer = AddPadding(data);

        byte* ivBytes = &iv[0];
        byte* keyBytes = &key[0];
        byte* dataBytes = &dataBuffer[0];

        AES_ctx context;
        AES_init_ctx_iv(&context, keyBytes, ivBytes);
        AES_CBC_encrypt_buffer(&context, dataBytes, dataBuffer.size());

        // Prepend IV to the data buffer
        dataBuffer.insert(dataBuffer.begin(), iv.begin(), iv.end());

        return dataBuffer;

    }

    std::vector<byte> StegCrypt::Decrypt(const std::vector<byte>& pass, const std::vector<byte> data) {

        // Create a random number generator with seed 0 for the IV
        RNG rng(0);

        // TODO Use a KDF instead (PBKDF2)
        std::vector<byte> key = DeriveKey(pass, rng);

        // IV is BLOCK_SIZE bytes long
        std::vector<byte> iv(data.begin(), data.begin() + BLOCK_SIZE);

        // Clip off the IV from the front
        std::vector<byte> dataBuffer(data.begin() + BLOCK_SIZE, data.end());

        byte* ivBytes = &iv[0];
        byte* keyBytes = &key[0];
        byte* dataBytes = &dataBuffer[0];

        AES_ctx context;
        AES_init_ctx_iv(&context, keyBytes, ivBytes);
        AES_CBC_decrypt_buffer(&context, dataBytes, dataBuffer.size());

        return RemovePadding(dataBuffer);

    }

    std::vector<byte> StegCrypt::GetIV(RNG& rng) {
        std::vector<byte> iv(IV_LENGTH);
        uint64_t rand64 = rng.Next();
        rand64 <<= 32;
        rand64 |= rng.Next();
        int k = 0;
        for (int i = 0; i < 2; i++) {
            iv[k++] = ((rand64 >> 56) & 0xFF);
            iv[k++] = ((rand64 >> 40) & 0xFF);
            iv[k++] = ((rand64 >> 32) & 0xFF);
            iv[k++] = ((rand64 >> 48) & 0xFF);
            iv[k++] = ((rand64 >> 24) & 0xFF);
            iv[k++] = ((rand64 >> 16) & 0xFF);
            iv[k++] = ((rand64 >> 8) & 0xFF);
            iv[k++] = (rand64 & 0xFF);
        }
        return iv;
    }

    std::vector<byte> StegCrypt::DeriveKey(const std::vector<byte>& pass, RNG& rng) {

        // Generate a 16 byte (128 bit) cryptographic salt
        constexpr uint32_t saltSize = BLOCK_SIZE;
        byte saltBytes[saltSize];
        for (int i = 0; i < BLOCK_SIZE; i++) {
            saltBytes[i] = rng.Next() & 0xFF;
        }

        // Array to hold the resulting key bytes
        constexpr uint32_t keySize = BLOCK_SIZE;
        byte keyBytes[keySize];

        // Derive key using Argon2
        argon2i_hash_raw(2, 1 << 8, 1, &pass[0], pass.size(), saltBytes, saltSize, keyBytes, keySize);

        // Convert the key bytes array to a vector
        std::vector<byte> key(keyBytes, keyBytes + BLOCK_SIZE);

        return key;

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
