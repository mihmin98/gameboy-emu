#ifndef __ROM_H__
#define __ROM_H__

#pragma once
#include <cstdint>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>

enum MBC { None, MBC1, MBC2, MMM01, MBC3, MBC5, MBC6, MBC7, HuC1, HuC3 };

class ROM
{
  public:
    uint8_t *rom;
    uint32_t romFileSize;

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

    /* MBC VARIABLES */

    bool ramEnable;
    uint8_t currentROMBank;
    uint8_t currentRAMBank;

    // NOTE: To switch banks, there will be mem writes to a ROM area
    // The writes are intercepted, no ROM will be written and the MBC controls
    // the switching

    ROM();

    bool loadROM(std::string romPath);

    /* CARTRIDGE READ AND WRITE */

    uint8_t readmem(uint16_t addr);
    void writemem(uint16_t addr);

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
};

#endif // __ROM_H__
