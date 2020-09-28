#include "stegosaurus_pch.h"
#include "StegCrypt.h"

#include "osrng.h"
#include <iostream>
#include <cstdlib>
#include "cryptlib.h"
#include "hex.h"
#include "filters.h"
#include "aes.h"
#include "modes.h"

namespace Steg {

    std::vector<byte> StegCrypt::Encrypt(const std::string& keyString, const std::vector<byte> inputData) {
        CryptoPP::AutoSeededRandomPool rnd;

        // Generate a random key
        CryptoPP::SecByteBlock key((byte*)keyString.c_str(), CryptoPP::AES::DEFAULT_KEYLENGTH);
        rnd.GenerateBlock(key, key.size());

        // Generate a random IV
        CryptoPP::SecByteBlock iv(CryptoPP::AES::BLOCKSIZE);
        rnd.GenerateBlock(iv, iv.size());

        const byte* inputBytes = &inputData[0];
        size_t messageLen = inputData.size();

        byte* outputBytes = new byte[messageLen];

        CryptoPP::CFB_Mode<CryptoPP::AES>::Encryption cfbEncryption(key, key.size(), iv);
        cfbEncryption.ProcessData(outputBytes, inputBytes, messageLen);

        // This may not work
        std::vector<byte> outputData(messageLen);
        std::copy(outputBytes, outputBytes + messageLen, std::back_inserter(outputData));

        delete[] outputBytes;

        return outputData;
    }

    std::vector<byte> StegCrypt::Decrypt(const std::string& key, const std::vector<byte> data) {

    }

}
