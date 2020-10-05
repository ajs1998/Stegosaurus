#include "stegosaurus_pch.h"
#include "StegCrypt.h"

#include <aes.hpp>

#include <ctime>

namespace Steg {

    // These methods will handle the IV in the background
    std::vector<byte> StegCrypt::Encrypt(const std::vector<byte>& key, const std::vector<byte> data) {

        int len;

        const std::vector<byte> ivBytes = GetIV();

        const byte* keyBytes = &key[0];
        const byte* dataBytes = &data[0];

        // The output data. It begins with the IV bytes
        std::vector<byte> encryptedData(ivBytes);
        byte* encDataBegin = &encryptedData[0] + IV_LENGTH;

        return encryptedData;

    }

    std::vector<byte> StegCrypt::Decrypt(const std::vector<byte>& key, const std::vector<byte> data) {

        int len;

        const byte* ivBytes = &data[0];

        const byte* keyBytes = &key[0];
        const byte* dataBytes = &data[0];

        // The output data
        std::vector<byte> decryptedData;
        byte* decDataBegin = &decryptedData[0];

        return decryptedData;

    }

    const std::vector<byte> StegCrypt::GetIV() {
        std::vector<byte> iv(IV_LENGTH);
        std::time_t sec = time(NULL);
        for (int i = 0; i < 2; i++) {
            iv.push_back((sec >> 56) & 0xFF);
            iv.push_back((sec >> 48) & 0xFF);
            iv.push_back((sec >> 40) & 0xFF);
            iv.push_back((sec >> 32) & 0xFF);
            iv.push_back((sec >> 24) & 0xFF);
            iv.push_back((sec >> 16) & 0xFF);
            iv.push_back((sec >> 8) & 0xFF);
            iv.push_back(sec & 0xFF);
        }
        return iv;
    }

}
