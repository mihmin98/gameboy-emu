#include "Config.hpp"
#include "Enums.hpp"
#include "GameBoy.hpp"
#include <iostream>
#include <string>
#include <experimental/filesystem>
#include "INIReader.h"

namespace fs = std::experimental::filesystem;

bool readConfig(char *execPath);
void printUsage(char *programName);

int main(int argc, char **argv)
{
    if (argc == 1) {
        std::cerr << "No args have been given\n";
        printUsage(argv[0]);
        return 1;
    }

    // Read config file
    if (!readConfig(argv[0])) {
        return 1;
    }

    EmulatorMode emulatorMode = EmulatorMode::DMG;
    std::string romPath;
    bool romPathSet = false;

    // Parse args
    for (int i = 1; i < argc; ++i) {
        if (argv[i][0] == '-') {
            // Flag
            switch (argv[i][1]) {
            case 'g':
                // Gameboy Flag
                if (!(i + 1 < argc)) {
                    std::cerr << "Bad number of args\n";
                    printUsage(argv[0]);
                    return 1;
                }

                if (strncmp(argv[i + 1], "dmg", 3) == 0) {
                    emulatorMode = DMG;
                } else if (strncmp(argv[i + 1], "cgb", 3) == 0) {
                    emulatorMode = CGB;
                } else {
                    std::cerr << "Bad gameboy type given\n";
                    printUsage(argv[0]);
                    return 1;
                }
                ++i;

                break;
            case 'w':
                // Window Size
                if (!(i + 1 < argc)) {
                    std::cerr << "Bad number of args\n";
                    printUsage(argv[0]);
                    return 1;
                }

                {
                    std::string windowSizeStr = argv[i + 1];
                    int windowSize;
                    try {
                        windowSize = std::stoi(windowSizeStr);
                    } catch (const std::invalid_argument &ia) {
                        std::cerr << "Bad window size arg\n";
                        printUsage(argv[0]);
                        return 1;
                    }

                    Config::getInstance()->setWindowSize(windowSize);
                    ++i;
                }
                break;
            case 'b':
                // Bootrom
                if (!(i + 1 < argc)) {
                    std::cerr << "Bad number of args\n";
                    printUsage(argv[0]);
                    return 1;
                }

                {
                    std::string bootromPath = argv[i + 1];

                    Config::getInstance()->setBootromPath(bootromPath);
                    Config::getInstance()->setUseBootrom(true);

                    ++i;
                }
                break;
            case 'h':
                // Help
                printUsage(argv[0]);
                return 0;
                break;
            default:
                std::cerr << "Bad args given\n";
                printUsage(argv[0]);
                return 1;
                break;
            }
        }

        else {
            // ROM
            if (romPathSet) {
                std::cerr << "Bad args given\n";
                printUsage(argv[0]);
                return 1;
            }

            romPath = argv[i];
            romPathSet = true;
        }
    }

    if (!romPathSet) {
        std::cerr << "No ROM file has been provided\n";
        printUsage(argv[0]);
        return 1;
    }

    GameBoy gb = GameBoy(emulatorMode);
    gb.romPath = romPath;

    gb.initSDL();
    gb.run();

    return 0;
}

bool readConfig(char *execPath)
{
    // Check if config file exists
    std::string configPathString = std::string(execPath) + ".ini";
    fs::path configPath = fs::path(configPathString);

    // If yes then read it
    if (fs::exists(configPath)) {
        INIReader reader(configPath);

        if (reader.ParseError() < 0) {
            std::cerr << "Error reading " << configPath << '\n';
            return false;
        }

        Config *config = Config::getInstance();
        
        int windowSize = reader.GetInteger("General", "windowSize", config->getWindowSize());
        if (windowSize != config->getWindowSize()) {
            config->setWindowSize(windowSize);
        }

        int audioBatchCycles = reader.GetInteger("General", "audioBatchCycles", config->getAudioBatchCycles());
        if (audioBatchCycles != config->getAudioBatchCycles()) {
            config->setAudioBatchCycles(audioBatchCycles);
        }

        float audioVolume = reader.GetReal("General", "audioVolume", config->getAudioVolume());
        if (audioVolume != config->getAudioVolume()) {
            config->setAudioVolume(audioVolume);
        }

        bool useBootrom = reader.GetBoolean("General", "useBootrom", config->getUseBootrom());
        if (useBootrom != config->getUseBootrom()) {
            config->setUseBootrom(useBootrom);
        }

        std::string bootromPath = reader.GetString("General", "bootromPath", config->getBootromPath());
        if (bootromPath != config->getBootromPath()) {
            config->setBootromPath(bootromPath);
        }

        bool printPerformanceInfo = reader.GetBoolean("General", "printPerformanceInfo", config->getPrintPerformanceInfo());
        if (printPerformanceInfo != config->getPrintPerformanceInfo()) {
            config->setPrintPerformanceInfo(printPerformanceInfo);
        }
    }

    // else, create it
    else {
        std::string configContents = Config::getInstance()->getConfigAsString();
        FILE *f = fopen(configPath.c_str(), "w");

        if (f == NULL) {
            std::cerr << "Could not create config file " << configPath << "\n";
            return false;
        }

        fwrite(configContents.c_str(), sizeof(char), configContents.length(), f);

        fclose(f);
    }

    return true;
}

// program name should be argv[0]
void printUsage(char *programName)
{
    std::cout << "Usage: " << programName << " rom_path [flags]\n"
              << "Flags:\n"
              << "\t-g dmg | cgb: Selects gameboy mode: DMG or CGB. By default DMG is selected\n"
              << "\t-w windowSize: How big should the window be compared to the gameboy's "
                 "resolution of 160x144\n"
              << "\t-b bootromPath: Path to the DMG bootrom\n"
              << "\t-h: Prints this message\n";
}
