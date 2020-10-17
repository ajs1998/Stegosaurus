#pragma once

#include <StegosaurusEngine/Core.h>

#include "../RNG.h"

#include <string>
#include <vector>

namespace Steg {

    class StegCrypt {

    public:

        static constexpr uint32_t BLOCK_SIZE = 16;

        static constexpr uint32_t IV_LENGTH = BLOCK_SIZE;

        static std::vector<byte> Encrypt(const std::vector<byte>& key, const std::vector<byte> inputBytes);

        static std::vector<byte> Decrypt(const std::vector<byte>& key, const std::vector<byte> inputBytes);

    private:

        static std::vector<byte> GetIV(RNG& rng);

        static std::vector<byte> DeriveKey(const std::vector<byte>& key, RNG& rng);

        static std::vector<byte> AddPadding(const std::vector<byte> data);

        static std::vector<byte> RemovePadding(const std::vector<byte> data);

    };

}
