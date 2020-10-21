#pragma once

#include <StegosaurusEngine/Core.h>

namespace Steg {

    class Application {

    public:

        Application(int argc, char** argv);

        virtual ~Application();

    protected:
        
        int argc;

        char** argv;

    };

    Application* CreateApplication(int argc, char** argv);

}
