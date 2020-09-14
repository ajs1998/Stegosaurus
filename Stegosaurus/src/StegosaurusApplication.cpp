#include "StegosaurusApplication.h"

#include <StegosaurusEngine/StegEngine.h>

#include "StegosaurusEngine/Image/Image.h"
#include "StegosaurusEngine/Image/RGBImage.h"
#include "StegosaurusEngine/Image/GrayImage.h"

#include <iostream>

StegosaurusApplication::StegosaurusApplication() {
    Steg::Image image("C:/Users/Alex/Desktop/In.png");
    std::vector<byte> data(5, 64);
    Steg::StegEngine::Encode(image, data, { 2, false, false });
}

StegosaurusApplication::~StegosaurusApplication() {
    // Application Destructor
}

// Implemented from StegosaurusEngine/EntryPoint.h
Steg::Application* Steg::CreateApplication() {
    return new StegosaurusApplication();
}
