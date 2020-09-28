#pragma once

#include <StegosaurusEngine/Core.h>

#include <string>
#include <vector>

namespace Steg {

    class StegCrypt {

    public:

        static std::vector<byte> Encrypt(const std::string& key, const std::vector<byte> inputBytes);

        static std::vector<byte> Decrypt(const std::string& key, const std::vector<byte> inputBytes);

    private:



    };

}
