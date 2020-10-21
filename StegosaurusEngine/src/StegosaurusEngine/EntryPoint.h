#pragma once

#include <StegosaurusEngine/Core.h>

extern Steg::Application* Steg::CreateApplication(int argc, char** argv);

int main(int argc, char** argv) {

    auto app = Steg::CreateApplication(argc, argv);

    delete app;

}
