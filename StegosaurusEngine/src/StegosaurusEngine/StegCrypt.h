#pragma once

#include <StegosaurusEngine/Core.h>

#include <string>
#include <vector>

namespace Steg {

    class StegCrypt {

    public:

        static std::vector<byte> Encrypt(const std::vector<byte>& key, const std::vector<byte> inputBytes);

        static std::vector<byte> Decrypt(const std::vector<byte>& key, const std::vector<byte> inputBytes);

    private:

        static constexpr uint32_t IV_LENGTH = 16;

        static const std::vector<byte> GetIV();

    };

}
