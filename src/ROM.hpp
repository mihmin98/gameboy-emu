#ifndef __ROM_H__
#define __ROM_H__

#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <cstdint>

enum MBC {None, MBC1, MBC2, MMM01, MBC3, MBC5, MBC6, MBC7, HuC1, HuC3};

class ROM 
{
  public:
    uint8_t *rom;
    uint32_t romFileSize;
     
    /* HEADER FLAGS */
    
    bool nintendoLogoOK;
    std::string gameTitle;
    std::string manufacturerCode;
    uint8_t gbcFlag;
    uint8_t sgbFlag;
    uint8_t licenseeCode;
    MBC mbc;
    uint32_t romSize;
    uint8_t romBanks;
    uint32_t ramSize;
    uint8_t ramBanks;
    uint8_t destinationCode;
    uint8_t romVersion;
    uint16_t headerChecksum;
    uint16_t globalChecksum;

    uint8_t currentROMBank;
    uint8_t currentRAMBank;

    // NOTE: To switch banks, there will be mem writes to a ROM area
    // The writes are intercepted, no ROM will be written and the MBC controlls
    // the switching

    uint8_t ramEnable;

    ROM();

    bool loadROM(std::string romPath);

    uint8_t readmem(uint16_t addr);
    void writemem(uint16_t addr);

    /* HEADER FLAG GETTERS */

    bool checkNintendoLogo();
    void getGameTitle();
    void getManufacturerCode();
    void getGBCFlag();
    void getNewLicenseeCode();
    void getSGBFlag();
    void getCartridgeType(); // This sets the header flags regarding MBC and other hardware
    void getROMSize(); // Sets the ROM size and number of memory banks
    void getRAMSize(); // Sets RAM size and number of RAM banks
    void getDestionationCode(); // 0 if sold in JP, 1 if elsewhere
    void getOldLicenseeCode();
    void getROMVersion();
    void getHeaderChecksum();
    void getGlobalChecksum();
};

#endif // __ROM_H__

