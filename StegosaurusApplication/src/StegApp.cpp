#include "StegApp.h"

#include <StegosaurusEngine/StegEngine.h>

#include <iostream>

#include "StegosaurusEngine/Image/Image.h"
#include "StegosaurusEngine/Image/RGBImage.h"
#include "StegosaurusEngine/Image/GrayImage.h"
#include "StegosaurusEngine/Crypt/StegCrypt.h"

#include "anyoption.h"

StegApp::StegApp(int argc, char** argv) : Application(argc, argv) {

    MainLoop();

}

StegApp::~StegApp() {
    // Application Destructor
}

void StegApp::MainLoop() {

    AnyOption opt = AnyOption();

    opt.addUsage("Usage: ./steg [options] -I <input file> -D <data file> -O <output file>");
    opt.addUsage("");
    opt.addUsage("OPTIONS:");
    opt.addUsage("  -h  --help                      Prints this help message");
    opt.addUsage("  -a  --use-alpha                 Encode data in alpha channel");
    opt.addUsage("  -d  --depth <depth>             Bit depth of hidden data");
    opt.addUsage("  -e  --encrypt                   Encrypt data before encoding");
    opt.addUsage("    -p  --pass <password>         Encryption password");
    opt.addUsage("    -g  --algo <algorithm>        Encryption algorithm");
    opt.addUsage("  -I  --in-file <filename>        The image to hide data in");
    opt.addUsage("  -D  --data-file <filename>      Binary file that contains the data to be hidden");
    opt.addUsage("  -O  --out-file <filename>       The resulting image");
    opt.addUsage("");
    opt.addUsage("ALLOWED VALUES:");
    opt.addUsage("  Depth (8-bit images):           1, 2, 4, 8, (16 for 16 bit images)");
    opt.addUsage("  Algorithm:                      AES128, AES192, AES256");

    opt.printUsage();

    opt.processCommandArgs(argc, argv);

}

// Implemented from StegosaurusEngine/EntryPoint.h
Steg::Application* Steg::CreateApplication(int argc, char** argv) {
    return new StegApp(argc, argv);
}
