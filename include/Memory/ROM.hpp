#ifndef __ROM_H__
#define __ROM_H__

#pragma once
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <experimental/filesystem>
#include <fstream>
#include <iostream>
#include <string>

#define ROM_RTC_T_CYCLES_UNTIL_TICK 128
#define ROM_RTC_TICKS_UNTIL_INCREMENT 32678

namespace fs = std::experimental::filesystem;

enum MBC { None, MBC1, MBC2, MMM01, MBC3, MBC5, MBC6, MBC7, HuC1, HuC3 };

class ROM
{
  public:
    uint8_t *rom;
    uint8_t *ram;
    uint32_t romFileSize;
    fs::path romFilePath;
    fs::path saveFilePath;
    FILE *saveFile = NULL;
    uint8_t bootrom[256];
    bool bootromActive;

    /* ROM CHECK FLAGS */

    bool nintendoLogoOk;
    bool headerChecksumOk;
    bool globalchecksumOk;

    /* HEADER FLAGS */

    std::string gameTitle;
    uint8_t gbcFlag;
    uint8_t sgbFlag;
    MBC mbc;
    uint32_t romSize;
    uint8_t romBanks;
    uint32_t ramSize;
    uint8_t ramBanks;
    uint8_t romVersion;
    uint16_t headerChecksum;
    uint16_t globalChecksum;

    /* CARTRIDGE INFO */

    bool cartridgeBattery;
    bool cartridgeRam;
    bool cartridgeTimer;
    bool isMulticart; // TODO

    /* MBC VARIABLES */

    bool ramEnable;
    uint16_t currentROMBank;
    uint8_t currentRAMBank; // In MBC1 it can be used to select ROM banks
    uint8_t bankMode;

    // NOTE: To switch banks, there will be mem writes to a ROM area
    // The writes are intercepted, no ROM will be written and the MBC controls
    // the switching

    /* TIMER REGISTERS */
    // TODO: Implement the actual timer

    uint8_t rtcS;
    uint8_t rtcM;
    uint8_t rtcH;
    uint8_t rtcDL;
    uint8_t rtcDH;
    uint8_t rtcLatchClockLastWritten;
    bool rtcLatch;
    uint64_t rtcLatchedSeconds;
    uint16_t rtcNumCycles;

    ROM();
    ~ROM();

    bool loadROM(std::string romPath);
    bool loadSaveFile(fs::path savePath);
    bool saveRam();
    bool loadBootrom(std::string bootromPath);
    void disableBootrom();

    /* ROM CHECKS */

    bool checkNintendoLogo();
    bool checkHeaderChecksum();
    bool checkGlobalChecksum();

    /* HEADER FLAG GETTERS */

    void readHeader();

    void getGameTitle();
    void getGBCFlag();
    void getSGBFlag();
    void getCartridgeType();
    void getROMSize();
    void getRAMSize();
    void getROMVersion();
    void getHeaderChecksum();
    void getGlobalChecksum();

    /* CARTRIDGE READ AND WRITE */

    uint8_t readmem(uint16_t addr);
    void writemem(uint8_t val, uint16_t addr);

    uint8_t readmemNoMBC(uint16_t addr);
    void writememNoMBC(uint8_t val, uint16_t addr);

    uint8_t readmemMBC1(uint16_t addr);
    void writememMBC1(uint8_t val, uint16_t addr);

    uint8_t readmemMBC2(uint16_t addr);
    void writememMBC2(uint8_t val, uint16_t addr);

    uint8_t readmemMBC3(uint16_t addr);
    void writememMBC3(uint8_t val, uint16_t addr);

    uint8_t readmemMBC5(uint16_t addr);
    void writememMBC5(uint8_t val, uint16_t addr);

    /* MBC3 RTC FUNCTIONS */

    // Should be called once every ROM_RTC_T_CYCLES_UNTIL_TICK t cycles
    void cycleRtc();
    void incrementRtc(uint64_t numSeconds);
};

#endif // __ROM_H__
