#include "StegApp.h"

#include <StegosaurusEngine/StegEngine.h>

#include "StegosaurusEngine/Image/Image.h"
#include "StegosaurusEngine/Image/RGBImage.h"
#include "StegosaurusEngine/Image/GrayImage.h"
#include "StegosaurusEngine/StegCrypt.h"

#include <iostream>

StegApp::StegApp() {

    Steg::Image image("C:/Users/Alex/Desktop/In.png");
    std::vector<byte> key({ 2, 3, 4, 5 });
    std::vector<byte> data(207, (byte)228); // 228 = 11100100
    Steg::StegEngine::Encode(image, Steg::StegCrypt::Encrypt(key, data), { 2, false });
    image.SaveImage("C:/Users/Alex/Desktop/Out.png");

}

StegApp::~StegApp() {
    // Application Destructor
}

// Implemented from StegosaurusEngine/EntryPoint.h
Steg::Application* Steg::CreateApplication() {
    return new StegApp();
}
