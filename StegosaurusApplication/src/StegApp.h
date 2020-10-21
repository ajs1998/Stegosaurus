#pragma once

#include "Stegosaurus.h"

class StegApp : public Steg::Application {

public:

    StegApp(int argc, char** argv);

    ~StegApp();

    void MainLoop();

};
