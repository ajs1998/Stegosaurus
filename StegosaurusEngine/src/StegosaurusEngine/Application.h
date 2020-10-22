#pragma once

#include <StegosaurusEngine/Core.h>

namespace Steg {

    class Application {

    public:

        Application();

        virtual ~Application();

    };

    Application* CreateApplication(int argc, char** argv);

}
