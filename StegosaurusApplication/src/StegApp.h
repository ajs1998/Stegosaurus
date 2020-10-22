#pragma once

#include "Stegosaurus.h"

#include "anyoption.h"

class StegApp : public Steg::Application {

public:

    StegApp(int argc, char** argv);

    ~StegApp();

private:

    AnyOption* GetOptions(int argc, char** argv);

    std::string ValueToString(char* value);

};
