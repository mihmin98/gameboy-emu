#include "Config.hpp"

Config *Config::instance = nullptr;

Config::Config()
{
    windowSize = 4;
    audioBatchCycles = 4;
    audioVolume = 1.0;
    useBootrom = false;
    bootromPath = "";
    printPerformanceInfo = false;
}

Config *Config::getInstance()
{
    if (instance == nullptr) {
        instance = new Config();
    }

    return instance;
}

std::string Config::getConfigAsString()
{
    std::string s = "[General]\nwindowSize=" + std::to_string(windowSize) +
                    "\naudioBatchCycles=" + std::to_string(audioBatchCycles) +
                    "\naudioVolume=" + std::to_string(audioVolume) +
                    "\nuseBootrom=" + std::to_string(useBootrom) + "\nbootromPath=" + bootromPath +
                    "\nprintPerformanceInfo=" + std::to_string(printPerformanceInfo);

    return s;
}

int Config::getWindowSize() { return windowSize; }

int Config::getAudioBatchCycles() { return audioBatchCycles; }

float Config::getAudioVolume() { return audioVolume; }

bool Config::getUseBootrom() { return useBootrom; }

std::string Config::getBootromPath() { return bootromPath; }

bool Config::getPrintPerformanceInfo() { return printPerformanceInfo; }

void Config::setWindowSize(int windowSize) { this->windowSize = windowSize; }

void Config::setAudioBatchCycles(int audioBatchCycles)
{
    this->audioBatchCycles = audioBatchCycles;
}

void Config::setAudioVolume(float audioVolume) { this->audioVolume = audioVolume; }

void Config::setUseBootrom(bool useBootrom) { this->useBootrom = useBootrom; }

void Config::setBootromPath(std::string bootromPath) { this->bootromPath = bootromPath; }

void Config::setPrintPerformanceInfo(bool printPerformanceInfo)
{
    this->printPerformanceInfo = printPerformanceInfo;
}
