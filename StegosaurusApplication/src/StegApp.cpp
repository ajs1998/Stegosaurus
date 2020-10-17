#include "StegApp.h"

#include <StegosaurusEngine/StegEngine.h>

#include "StegosaurusEngine/Image/Image.h"
#include "StegosaurusEngine/Image/RGBImage.h"
#include "StegosaurusEngine/Image/GrayImage.h"
#include "StegosaurusEngine/Crypt/StegCrypt.h"

#include <iostream>

StegApp::StegApp() {

    Steg::Image image("C:/Users/Alex/Desktop/In.png");
    std::vector<byte> key({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 });
    std::vector<byte> data(111, (byte)0xFF);
    
    Steg::EncoderSettings settings;
    settings.DataDepth = 1;
    settings.EncodeInAlpha = false;
    settings.EncryptPayload = true;
    settings.EncryptionKey = key;

    if (!Steg::StegEngine::Encode(image, data, settings)) {
        std::cerr << "Could not encode image" << std::endl;
        return;
    }
    image.SaveImage("C:/Users/Alex/Desktop/Out.png");

    auto decodedBytes = Steg::StegEngine::Decode(image, key);

    int breakpoint = 0;

}

StegApp::~StegApp() {
    // Application Destructor
}

// Implemented from StegosaurusEngine/EntryPoint.h
Steg::Application* Steg::CreateApplication() {
    return new StegApp();
}
