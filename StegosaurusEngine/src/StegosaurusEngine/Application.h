#pragma once

#include <StegosaurusEngine/Core.h>

namespace Steg {

    class Application {

    public:

        Application();

        virtual ~Application();

        void Run();

    };

    Application* CreateApplication();

}
