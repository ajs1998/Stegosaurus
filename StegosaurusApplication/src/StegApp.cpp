#include "StegApp.h"

#include <StegosaurusEngine/StegEngine.h>

#include "StegosaurusEngine/Image/Image.h"
#include "StegosaurusEngine/Image/RGBImage.h"
#include "StegosaurusEngine/Image/GrayImage.h"
#include "StegosaurusEngine/Crypt/StegCrypt.h"

#include <iostream>

StegApp::StegApp(int argc, char** argv) : Application(argc, argv) {

    MainLoop();

}

StegApp::~StegApp() {
    // Application Destructor
}

void StegApp::MainLoop() {

    // This is temporary
    for (int i = 1; i < argc; i++) {
        std::cout << argv[i] << " ";
    }

}

// Implemented from StegosaurusEngine/EntryPoint.h
Steg::Application* Steg::CreateApplication(int argc, char** argv) {
    return new StegApp(argc, argv);
}
