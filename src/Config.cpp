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
    useCustomDMGPalette = false;

    for (uint8_t i = 0; i < 4; ++i) {
        uint8_t val = 255 - (i * (255 / 3));
        bgCustomDMGPalette[i] = Color(val, val, val);
        obp0CustomDMGPalette[i] = Color(val, val, val);
        obp1CustomDMGPalette[i] = Color(val, val, val);
    }
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
    std::string s =
        "[General]\nwindowSize=" + std::to_string(windowSize) +
        "\naudioBatchCycles=" + std::to_string(audioBatchCycles) +
        "\naudioVolume=" + std::to_string(audioVolume) +
        "\nuseBootrom=" + std::to_string(useBootrom) + "\nbootromPath=" + bootromPath +
        "\nprintPerformanceInfo=" + std::to_string(printPerformanceInfo) +
        "\nuseCustomDMGPalette=" + std::to_string(useCustomDMGPalette) +
        "\n\n[Colors]\n; Colors should be given in the following format: #rrggbb\n\n" +
        "bgColor0=#ffffff\nbgColor1=#aaaaaa\nbgColor2=#555555\nbgColor3=#000000\n\n" +
        "obp0Color0=#ffffff\nobp0Color1=#aaaaaa\nobp0Color2=#555555\nopb0Color3=#000000\n\n" +
        "obp1Color0=#ffffff\nobp1Color1=#aaaaaa\nobp1Color2=#555555\nobp1Color3=#000000";

    return s;
}

int Config::getWindowSize() { return windowSize; }

int Config::getAudioBatchCycles() { return audioBatchCycles; }

float Config::getAudioVolume() { return audioVolume; }

bool Config::getUseBootrom() { return useBootrom; }

std::string Config::getBootromPath() { return bootromPath; }

bool Config::getPrintPerformanceInfo() { return printPerformanceInfo; }

bool Config::getUseCustomDMGPalette() {
    return useCustomDMGPalette;
}

Color Config::getBgCustomDMGPalette(int index) {
    return bgCustomDMGPalette[index];
}

Color Config::getObp0CustomDMGPalette(int index) {
    return obp0CustomDMGPalette[index];
}

Color Config::getObp1CustomDMGPalette(int index) {
    return obp1CustomDMGPalette[index];
}

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

void Config::setUseCustomDMGPalette(bool useCustomDMGPalette) {
    this->useCustomDMGPalette = useCustomDMGPalette;
}

void Config::setBgCustomDMGPalette(int index, Color color) {
    bgCustomDMGPalette[index] = color;
}

void Config::setObp0CustomDMGPalette(int index, Color color) {
    obp0CustomDMGPalette[index] = color;
}

void Config::setObp1CustomDMGPalette(int index, Color color) {
    obp1CustomDMGPalette[index] = color;
}
