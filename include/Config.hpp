#ifndef __CONFIG_H__
#define __CONFIG_H__

#pragma once
#include <cstdint>
#include <string>
#include "Color.hpp"

class Config
{
  private:
    static Config *instance;

    // data
    int windowSize = 4;
    int audioBatchCycles = 4;
    float audioVolume = 1.0;
    bool useBootrom = false;
    std::string bootromPath;
    bool printPerformanceInfo;
    bool useCustomDMGPalette;

    Color bgCustomDMGPalette[4];
    Color obp0CustomDMGPalette[4];
    Color obp1CustomDMGPalette[4];

    Config();

  public:
    static Config *getInstance();

    std::string getConfigAsString();

    int getWindowSize();
    int getAudioBatchCycles();
    float getAudioVolume();
    bool getUseBootrom();
    std::string getBootromPath();
    bool getPrintPerformanceInfo();
    bool getUseCustomDMGPalette();
    Color getBgCustomDMGPalette(int index);
    Color getObp0CustomDMGPalette(int index);
    Color getObp1CustomDMGPalette(int index);

    void setWindowSize(int windowSize);
    void setAudioBatchCycles(int audioBatchCycles);
    void setAudioVolume(float audioVolume);
    void setUseBootrom(bool useBootrom);
    void setBootromPath(std::string bootromPath);
    void setPrintPerformanceInfo(bool printPerformanceInfo);
    void setUseCustomDMGPalette(bool useCustomDMGPalette);
    void setBgCustomDMGPalette(int index, Color color);
    void setObp0CustomDMGPalette(int index, Color color);
    void setObp1CustomDMGPalette(int index, Color color);
};

#endif // __CONFIG_H__
