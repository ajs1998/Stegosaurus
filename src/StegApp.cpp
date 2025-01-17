#include "StegApp.h"

#include <StegEngine.h>

#include <iostream>

#include "StegosaurusEngine/Image/Image.h"
#include "StegosaurusEngine/Image/RGBImage.h"
#include "StegosaurusEngine/Image/GrayImage.h"
#include "StegosaurusEngine/Crypt/StegCrypt.h"
#include "StegosaurusEngine/StegTimer.h"

StegApp::StegApp(int argc, char** argv) {

    // Start the Total Timer
    Steg::StegTimer::StartTimer(Steg::StegTimer::TimerLabel::TOTAL);

    // Get all command line arguments
    std::unique_ptr<AnyOption> options = GetOptions(argc, argv);

    // If user uses the "help" flag, print the help message then quit.
    if (options->getFlag('h') || !options->hasOptions()) {
        options->printUsage();
        exit(0);
    }

    // Get all options from the user
    std::string operation = ValueToString(options->getArgv(0));
    std::string passOption = ValueToString(options->getValue(ShortPass));
    std::string inFileOption = ValueToString(options->getValue(ShortIn));
    std::string dataFileOption = ValueToString(options->getValue(ShortData));
    std::string outFileOption = ValueToString(options->getValue(ShortOut));
    std::string depthOption = ValueToString(options->getValue(ShortDepth));
    std::string algoOption = ValueToString(options->getValue(ShortAlgo));
    bool useAlphaOption = options->getFlag(ShortAlpha);
    bool encryptOption = passOption != ""; // Encryption is enabled if a password is provided

    // Parse depth
    uint32_t depth = 2;
    if (depthOption != "") {
        depth = std::stoi(depthOption);
    }
    if (depth != 1 && depth != 2 && depth != 4 && depth != 8 && depth != 16) {
        std::cerr << "Depth must be either 1, 2, 4, 8" << std::endl;
        exit(1);
    }

    // Parse algo (Default: AES128)
    Steg::StegCrypt::Algorithm algo;
    if (algoOption == "" || algoOption == "AES128") {
        algo = Steg::StegCrypt::Algorithm::ALGO_AES128;
        algoOption = "AES128"; // TODO this is bad but I don't want to algo.ToString() later.
    }
    else if (algoOption == "AES192") {
        algo = Steg::StegCrypt::Algorithm::ALGO_AES192;
    }
    else if (algoOption == "AES256") {
        algo = Steg::StegCrypt::Algorithm::ALGO_AES256;
    }
    else {
        std::cerr << "Invalid encryption algorithm: " << algoOption << std::endl;
        exit(1);
    }

    // Print these options back for confirmation
    if (operation == EncodeOp) {
        std::cout << "Operation: Encode" << std::endl;
        std::cout << "  Data Depth: " << depth << std::endl;
        std::cout << "  Use Alpha: " << (useAlphaOption ? "True" : "False") << std::endl;
        if (encryptOption) {
            std::cout << "  Encryption: Enabled" << std::endl;
            std::cout << "    Algorithm: " << algoOption << std::endl;
            std::cout << "    Password: " << passOption << std::endl;
        }
        else {
            std::cout << "  Encryption: Disabled" << std::endl;
        }
        std::cout << "  Input Image: " << inFileOption << std::endl;
        std::cout << "  Input Data File: " << dataFileOption << std::endl;
        std::cout << "  Output Image: " << outFileOption << std::endl;
    }
    else if (operation == DecodeOp) {
        std::cout << "Operation: Decode" << std::endl;
        if (encryptOption) {
            std::cout << "  Password: " << passOption << std::endl;
        }
        std::cout << "  Input Image: " << inFileOption << std::endl;
        std::cout << "  Output Data File: " << dataFileOption << std::endl;
    }
    else {
        std::cerr << "Invalid operation: " << operation << std::endl;
        exit(1);
    }

    // Ask user if we should proceed
    std::cout << "Proceed with these options? (y/n): ";

    string proceedStr;
    std::cin >> proceedStr;
    if (proceedStr != "Y" && proceedStr != "y") {
        // Operation is cancelled if the user enters anything other than 'Y' or 'y'
        std::cout << "Cancelled Operation" << std::endl;
        exit(0);
    }
    else {
        if (operation == EncodeOp) {
            std::cout << "Encoding and saving to " << outFileOption << ":" << std::endl;
        }
        else if (operation == DecodeOp) {
            std::cout << "Decoding and saving to " << dataFileOption << ":" << std::endl;
        }
    }

    // Parse pass
    std::vector<Steg::byte> passBytes;
    for (char c : passOption) {
        passBytes.push_back(reinterpret_cast<Steg::byte&>(c));
    }

    // Encode using StegEngine
    if (operation == EncodeOp) {

        // Create EncoderSettings and fill it with the proper options
        Steg::EncoderSettings encoderSettings;
        encoderSettings.EncodeInAlpha = useAlphaOption;
        encoderSettings.Encryption.EncryptPayload = encryptOption;
        encoderSettings.Encryption.EncryptionPassword = passBytes;

        encoderSettings.DataDepth = depth;

        // Get the input image
        Steg::Image image(inFileOption);

        // Read the data file into dataBytes 
        std::ifstream dataFile(dataFileOption, ios::binary | ios::ate);
        if (!dataFile) {
            std::cerr << "Cannot open file: " << dataFileOption << std::endl;
            exit(1);
        }
        int fileLength = (int)dataFile.tellg();
        std::vector<char> dataChars(fileLength);
        dataFile.seekg(0, ios::beg);
        dataFile.read(&dataChars[0], fileLength);
        std::vector<Steg::byte> dataBytes(dataChars.begin(), dataChars.end());

        // Check that dataFile will fit in image with the specified encoderSettings
        uint32_t availableBytes = Steg::StegEngine::CalculateAvailableBytes(image, encoderSettings);
        if (dataBytes.size() > availableBytes) {
            std::cerr << "The data in " << dataFileOption << " cannot be encoded in " << inFileOption << " with the specified settings" << std::endl;
            exit(1);
        }
        std::cout << "  Maximum input data file size for this image and these settings: " << (availableBytes / double(1000)) << "KB" << std::endl;
        std::cout << "  Ratio of overwritten available bytes: " << (fileLength / double(availableBytes) * 100) << "%" << std::endl;

        // Encode the image
        try {
            Steg::StegEngine::Encode(image, dataBytes, encoderSettings);
        }
        catch (const std::runtime_error& e) {
            std::cerr << e.what() << std::endl;
            exit(1);
        }

        // Finally, save the image
        image.SaveImage(outFileOption);

    }
    // Decode using StegEngine
    else if (operation == DecodeOp) {

        // Get the input image
        Steg::Image image(inFileOption);

        // Decode the image
        try {
            std::vector<Steg::byte> dataBytes = Steg::StegEngine::Decode(image, passBytes);

            // Finally, save the data file
            std::ofstream dataFile(dataFileOption, ios::binary);
            if (!dataFile) {
                cerr << "Cannot open file: " << dataFileOption << endl;
                exit(1);
            }
            dataFile.write(reinterpret_cast<char*>(&dataBytes[0]), dataBytes.size());
        }
        catch (std::exception& e) {
            cerr << e.what() << std::endl;
            exit(1);
        }

    }

    // End the Total Timer
    Steg::StegTimer::EndTimer(Steg::StegTimer::TimerLabel::TOTAL);

}

StegApp::~StegApp() {
    Steg::StegTimer::PrintTimers();
    // Application Destructor
}

std::unique_ptr<AnyOption> StegApp::GetOptions(int argc, char** argv) {

    std::unique_ptr<AnyOption> opt = std::make_unique<AnyOption>();

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
    opt->setOption(LongPass, ShortPass);
    opt->setOption(LongAlgo, ShortAlgo);
    opt->setOption(LongIn, ShortIn);
    opt->setOption(LongData, ShortData);
    opt->setOption(LongOut, ShortOut);

    opt->processCommandArgs(argc, argv);

    // TODO exit(1) if any invalid flags or options are encountered

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

int main(int argc, char** argv) {
    StegApp app = StegApp(argc, argv);
}
