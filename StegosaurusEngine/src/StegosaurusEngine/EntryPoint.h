#pragma once

#include <StegosaurusEngine/Core.h>

extern Steg::Application* Steg::CreateApplication();

int main(int argc, char** argv) {

    auto app = Steg::CreateApplication();

    app->Run();

    delete app;

}
