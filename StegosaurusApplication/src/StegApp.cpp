#include "StegApp.h"

#include <StegosaurusEngine/StegEngine.h>

#include <iostream>

#include "StegosaurusEngine/Image/Image.h"
#include "StegosaurusEngine/Image/RGBImage.h"
#include "StegosaurusEngine/Image/GrayImage.h"
#include "StegosaurusEngine/Crypt/StegCrypt.h"

StegApp::StegApp(int argc, char** argv) {

    // Get all command line arguments
    AnyOption* options = GetOptions(argc, argv);

    // If user uses the "help" flag, print the help message then quit.
    if (options->getFlag('h')) {
        options->printUsage();
        exit(0);
    }

    // Get the operation from the user
    std::string operation = ValueToString(options->getArgv(0));
    std::string passOption = ValueToString(options->getValue(ShortPass));
    std::string inFileOption = ValueToString(options->getValue(ShortIn));
    std::string dataFileOption = ValueToString(options->getValue(ShortData));

    // Parse pass
    std::vector<Steg::byte> passBytes;
    for (char c : passOption) {
        passBytes.push_back(reinterpret_cast<Steg::byte&>(c));
    }

    // Encode using StegEngine
    if (operation == EncodeOp) {

        // Get encoder-specific options
        bool useAlphaOption = options->getFlag(ShortAlpha);
        bool encryptOption = options->getFlag(ShortEncrypt);
        std::string depthOption = ValueToString(options->getValue(ShortDepth));
        std::string algoOption = ValueToString(options->getValue(ShortAlgo));
        std::string outFileOption = ValueToString(options->getValue(ShortOut));

        // Create EncoderSettings and fill it with the proper options
        Steg::EncoderSettings encoderSettings;
        encoderSettings.EncodeInAlpha = useAlphaOption;
        encoderSettings.Encryption.EncryptPayload = encryptOption;

        // Parse depth
        uint32_t depth = 2;
        if (depthOption != "") {
            depth = std::stoi(depthOption);
        }
        if (depth != 1 && depth != 2 && depth != 4 && depth != 8 && depth != 16) {
            std::cerr << "Depth must be either 1, 2, 4, 8" << std::endl;
            exit(1);
        }
        encoderSettings.DataDepth = depth;

        // Insert passBytes into encoderSettings
        encoderSettings.Encryption.EncryptionPassword = passBytes;

        // Parse algo (Default: AES128)
        if (algoOption == "" || algoOption == "AES128") {
            encoderSettings.Encryption.Algo = Steg::StegCrypt::Algorithm::ALGO_AES128;
        }
        else if (algoOption == "AES192") {
            encoderSettings.Encryption.Algo = Steg::StegCrypt::Algorithm::ALGO_AES192;
        }
        else if (algoOption == "AES256") {
            encoderSettings.Encryption.Algo = Steg::StegCrypt::Algorithm::ALGO_AES256;
        }
        else {
            std::cerr << "Invalid encryption algorithm: " << algoOption << std::endl;
            exit(1);
        }

        // Get the input image
        Steg::Image image(inFileOption);

        // Read the data input
        std::ifstream dataFile(dataFileOption, ios::in | ios::binary);
        std::vector<Steg::byte> dataBytes;
        while (Steg::byte b = Steg::byte(dataFile.get())) {
            dataBytes.push_back(b);
        }

        // Encode the image
        if (!Steg::StegEngine::Encode(image, dataBytes, encoderSettings)) {
            std::cerr << "Could not encode image!" << std::endl;
            exit(1);
        }

        // Finally, save the image
        image.SaveImage(outFileOption);

    }
    // Decode using StegEngine
    else if (operation == DecodeOp) {

        std::string outFileOption = ValueToString(options->getValue(ShortOut));

        // Get the input image
        Steg::Image image(inFileOption);

        // Decode the image
        std::vector<Steg::byte> data = Steg::StegEngine::Decode(image, passBytes);

        // Finally, save the data file
        // TODO Write data to file

    }
    // Invalid operation
    else {
        std::cerr << "Invalid operation: " << operation << std::endl;
        exit(1);
    }

    delete options;

}

StegApp::~StegApp() {
    // Application Destructor
}

AnyOption* StegApp::GetOptions(int argc, char** argv) {

    AnyOption* opt = new AnyOption();

    // Help message
    opt->addUsage("Usage: -> ./steg encode [Encoding Options] [Encryption Options] -I <input image> -D <data file> -O <output image>");
    opt->addUsage("          ./steg decode [Decryption Options] -I <input image> -D <data file>");
    opt->addUsage("          ./steg <-h | --help>   Prints this help message");
    opt->addUsage("");
    opt->addUsage("Encoding Options:");
    opt->addUsage("  -a  --use-alpha                Encode data in alpha channel (Default: False)");
    opt->addUsage("  -d  --depth <depth>            Bit depth of hidden data (Default: 2)");
    opt->addUsage("Encryption Options:");
    opt->addUsage("  -p  --pass <password>          Encryption password");
    opt->addUsage("  -g  --algo <algorithm>         Encryption algorithm (AES128, AES192, AES256) (Default: AES128)");
    opt->addUsage("Decryption Options:");
    opt->addUsage("  -p  --pass <password>          Decryption password (same as encryption password)");
    opt->addUsage("Other:");
    opt->addUsage("  -I  --in-image <filename>      The image to hide data in");
    opt->addUsage("  -D  --data-file <filename>     Binary file that contains the data to be hidden");
    opt->addUsage("  -O  --out-image <filename>     The resulting image");
    opt->addUsage("");
    opt->addUsage("Examples:");
    opt->addUsage("./steg encode -I \"in.png\" -D \"data.bin\" -O \"out.png\"");
    opt->addUsage("    Encodes the data in \"data.bin\" into \"in.png\" and stores the resulting image in \"out.png\"");
    opt->addUsage("");
    opt->addUsage("./steg encode -a -d 4 -p \"pass123\" -g AES192 -I \"in.png\" -D \"data.bin\" -O \"out.png\"");
    opt->addUsage("    Same as above, but also encode in alpha, set depth to 4 bits, and encrypt payload with a password using AES192.");
    opt->addUsage("");
    opt->addUsage("./steg decode -I \"in.png\" -D \"data.bin\"");
    opt->addUsage("    Decodes \"in.png\" and stores the resulting data in \"data.bin\"");
    opt->addUsage("");
    opt->addUsage("./steg decode -p \"pass123\" -I \"in.png\" -D \"data.bin\"");
    opt->addUsage("    Same as above, but decrypt using a password. The algorithm is automatically detected");

    opt->setFlag(LongHelp, ShortHelp);
    opt->setFlag(LongAlpha, ShortAlpha);
    opt->setOption(LongDepth, ShortDepth);
    opt->setFlag(LongEncrypt, ShortEncrypt);
    opt->setOption(LongPass, ShortPass);
    opt->setOption(LongAlgo, ShortAlgo);
    opt->setFileOption(LongIn, ShortIn);
    opt->setFileOption(LongData, ShortData);
    opt->setFileOption(LongOut, ShortOut);

    opt->processCommandArgs(argc, argv);

    return opt;

}

std::string StegApp::ValueToString(char* value) {
    std::string str;
    if (value == NULL) {
        return "";
    }
    else {
        return value;
    }
}

// Implemented from StegosaurusEngine/EntryPoint.h
Steg::Application* Steg::CreateApplication(int argc, char** argv) {
    return new StegApp(argc, argv);
}
