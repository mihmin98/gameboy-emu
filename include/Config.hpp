#ifndef __CONFIG_H__
#define __CONFIG_H__

#pragma once
#include <cstdint>
#include <string>

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

    void setWindowSize(int windowSize);
    void setAudioBatchCycles(int audioBatchCycles);
    void setAudioVolume(int audioVolume);
    void setUseBootrom(bool useBootrom);
    void setBootromPath(std::string bootromPath);
    void setPrintPerformanceInfo(bool printPerformanceInfo);
};

#endif // __CONFIG_H__
