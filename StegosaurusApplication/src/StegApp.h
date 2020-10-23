#pragma once

#include "Stegosaurus.h"

#include "anyoption.h"

constexpr auto EncodeOp = "encode";
constexpr auto DecodeOp = "decode";

constexpr auto LongHelp = "help";
constexpr auto ShortHelp = 'h';
constexpr auto LongAlpha = "use-alpha";
constexpr auto ShortAlpha = 'a';
constexpr auto LongDepth = "depth";
constexpr auto ShortDepth = 'd';
constexpr auto LongEncrypt = "encrypt";
constexpr auto ShortEncrypt = 'e';
constexpr auto LongPass = "pass";
constexpr auto ShortPass = 'p';
constexpr auto LongAlgo = "algo";
constexpr auto ShortAlgo = 'g';
constexpr auto LongIn = "in-file";
constexpr auto ShortIn = 'I';
constexpr auto LongData = "data-file";
constexpr auto ShortData = 'D';
constexpr auto LongOut = "out-file";
constexpr auto ShortOut = 'O';

class StegApp : public Steg::Application {

public:

    StegApp(int argc, char** argv);

    ~StegApp();

private:

    AnyOption* GetOptions(int argc, char** argv);

    std::string ValueToString(char* value);

};
