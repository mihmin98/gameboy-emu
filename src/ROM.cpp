#include "ROM.hpp"

ROM::ROM() 
{
    ramEnable = false;

    currentROMBank = 1;
    currentRAMBank = 1;
}

/**
 *  Loads the ROM at the specified path. Returns true on success
 */
bool ROM::loadROM(std::string romPath)
{
    std::ifstream romFile(romPath, std::ios::binary | std::ios::ate);

    if (romFile.is_open()) {
        // Get ROM file size
        romFileSize = romFile.tellg();
        romFile.seekg(0, std::ios::beg);

        // Allocate mem for ROM
        try {
            rom = new uint8_t[romFileSize];
        } catch (const std::bad_alloc &e) {
            std::cerr << "ERROR: Could not allocate memory for ROM: " << e.what() << "\n";
            return false;
        }

        romFile.read((char *)rom, romFileSize);
        romFile.close();

        // After the ROM has been loaded, read the header and run the checks
        readHeader();
        
        nintendoLogoOk = checkNintendoLogo();
        headerChecksumOk = checkHeaderChecksum();
        globalchecksumOk = checkGlobalChecksum();

        return true;
    } else {
        std::cerr << "ERROR: ROM " << romPath << " could not be opened\n";
        return false;
    }
}

/* ROM CHECKS */

/**
 *  Compares the logo at 0x0104 with the correct logo and returns the result
 */
bool ROM::checkNintendoLogo()
{
    uint8_t goodNintendoLogo[] = {0xCE, 0xED, 0x66, 0x66, 0xCC, 0x0D, 0x00, 0x0B, 0x03, 0x73,
                                  0x00, 0x83, 0x00, 0x0C, 0x00, 0x0D, 0x00, 0x08, 0x11, 0x1F,
                                  0x88, 0x89, 0x00, 0x0E, 0xDC, 0xCC, 0x6E, 0xE6, 0xDD, 0xDD,
                                  0xD9, 0x99, 0xBB, 0xBB, 0x67, 0x63, 0x6E, 0x0E, 0xEC, 0xCC,
                                  0xDD, 0xDC, 0x99, 0x9F, 0xBB, 0xB9, 0x33, 0x3E};
    if (memcmp(rom + 0x0104, goodNintendoLogo, 48 * sizeof(uint8_t)))
        return true;
    else
        return false;
}

/**
 *  Calculates the header checksum and compares it to the value in the header
 *  If the checksum is incorrect, the game shouldn't run
 */
bool ROM::checkHeaderChecksum()
{
    uint16_t sum = 0;
    uint16_t i = 0x0134;
    while (i <= 0x014C)
        sum = sum - rom[i++] - 1;

    return sum == headerChecksum;
}

/**
 *  Calculates the entire ROM checksum and compares it to the value in the header
 *  The Gameboy doesn't check this condition
 */
bool ROM::checkGlobalChecksum()
{
    uint16_t sum = 0;
    for (uint32_t i = 0; i < 0x014E; ++i)
        sum += rom[i];

    for (uint32_t i = 0x0150; i < romSize; ++i)
        sum += rom[i];

    return sum == globalChecksum;
}

/* HEADER FLAGS GETTERS */

/**
 *  Reads the header and sets the appropriate flags
 */
void ROM::readHeader()
{
    getGameTitle();
    getGBCFlag();
    getSGBFlag();
    getCartridgeType();
    getROMSize();
    getRAMSize();
    getROMVersion();
    getHeaderChecksum();
    getGlobalChecksum();
}

/**
 *  Sets the gameTitle according to the 16 bytes from 0x0134
 */
void ROM::getGameTitle()
{
    uint8_t title[16];
    memcpy(title, rom + 0x0134, 16 * sizeof(uint8_t));

    gameTitle.assign((char *)title);
}

/**
 *  Get byte at 0x0143
 *  0x80: GBC game is compatible with GB
 *  0xC0: Game works only on GBC
 */
void ROM::getGBCFlag() { gbcFlag = rom[0x0143]; }

/**
 *  Get byte at 0x0146
 *  0x00: No SGB
 *  0x03: SGB functions
 */
void ROM::getSGBFlag() { sgbFlag = rom[0x0146]; }

/**
 *  Get used memory controller and additional cartridge hardware from byte at 0x0147
 */
void ROM::getCartridgeType()
{
    uint8_t type = rom[0x0147];

    cartridgeBattery = false;
    cartridgeRam = false;
    cartridgeTimer = false;

    switch (type) {
    case 0x00:
        // ROM only
        mbc = MBC::None;
        break;
    case 0x01:
        // MBC1
        mbc = MBC::MBC1;
        break;
    case 0x02:
        // MBC1 + RAM
        mbc = MBC::MBC1;
        cartridgeRam = true;
        break;
    case 0x03:
        // MBC1 + RAM + BATTERY
        mbc = MBC::MBC1;
        cartridgeRam = true;
        cartridgeBattery = true;
        break;
    case 0x05:
        // MBC2
        mbc = MBC::MBC2;
        break;
    case 0x06:
        // MBC2 + BATTERY
        mbc = MBC::MBC2;
        cartridgeBattery = true;
        break;
    case 0x08:
        // ROM + RAM
        mbc = MBC::None;
        cartridgeRam = true;
        break;
    case 0x09:
        // ROM + RAM + BATTERY
        mbc = MBC::None;
        cartridgeRam = true;
        cartridgeBattery = true;
        break;
    case 0x0B:
        // MMM01
        mbc = MBC::MMM01;
        break;
    case 0x0C:
        // MMM01 + RAM
        mbc = MBC::MMM01;
        cartridgeRam = true;
        break;
    case 0x0D:
        // MMM01 + RAM + BATTERY
        mbc = MBC::MMM01;
        cartridgeRam = true;
        cartridgeBattery = true;
        break;
    case 0x0F:
        // MBC3 + TIMER + BATTERY
        mbc = MBC::MBC3;
        cartridgeTimer = true;
        cartridgeBattery = true;
        break;
    case 0x10:
        // MBC3 + TIMER + RAM + BATTERY
        mbc = MBC::MBC3;
        cartridgeTimer = true;
        cartridgeRam = true;
        cartridgeBattery = true;
        break;
    case 0x11:
        // MBC3
        mbc = MBC::MBC3;
        break;
    case 0x12:
        // MBC3 + RAM
        mbc = MBC::MBC3;
        cartridgeRam = true;
        break;
    case 0x13:
        // MBC3 + RAM + BATTERY
        mbc = MBC::MBC3;
        cartridgeRam = true;
        cartridgeBattery = true;
        break;
    case 0x19:
        // MBC5
        mbc = MBC::MBC5;
        break;
    case 0x1A:
        // MBC5 + RAM
        mbc = MBC::MBC5;
        cartridgeRam = true;
        break;
    case 0x1B:
        // MBC5 + RAM + BATTERY
        mbc = MBC::MBC5;
        cartridgeRam = true;
        cartridgeBattery = true;
        break;
    case 0x1C:
        // MBC5 + RUBMLE
        mbc = MBC::MBC5;
        break;
    case 0x1D:
        // MBC5 + RUBMLE + RAM
        mbc = MBC::MBC5;
        cartridgeRam = true;
        break;
    case 0x1E:
        // MBC5 + RUBMLE + RAM + BATTERY
        mbc = MBC::MBC5;
        cartridgeRam = true;
        cartridgeBattery = true;
        break;
    case 0x20:
        // MBC6
        mbc = MBC::MBC6;
        break;
    case 0x22:
        // MBC7 + SENSOR + RUMBLE + RAM + BATTERY
        mbc = MBC::MBC7;
        cartridgeRam = true;
        cartridgeBattery = true;
        break;
    case 0xFC:
        // POCKET CAMERA
        break;
    case 0xFD:
        // BANDAI TAMA5
        break;
    case 0xFE:
        // HuC3
        mbc = MBC::HuC3;
        break;
    case 0xFF:
        // HuC1 + RAM + BATTERY
        mbc = MBC::HuC3;
        cartridgeRam = true;
        cartridgeBattery = true;
        break;
    }
}

/**
 *  Reads the ROM size from byte 0x0148
 *  ROM size is calculated using the formula 32KB << byte
 *  ROM banks are equal to 2 << byte
 */
void ROM::getROMSize()
{
    uint8_t sizeByte = rom[0x0148];

    if (sizeByte <= 0x08) {
        romSize = 0x8000 << sizeByte;
        romBanks = 2 << sizeByte;
    }
}

/**
 *  Reads the external RAM size from byte at 0x0149
 *  NOTE: MBC2 has an external RAM size of 0, since the RAM is built in the MBC
 */
void ROM::getRAMSize()
{
    uint8_t sizeByte = rom[0x0149];

    switch (sizeByte) {
    case 0x00:
        // No RAM
        break;
    case 0x01:
        ramSize = 0x0800;
        break;
    case 0x02:
        ramSize = 0x1000;
        break;
    case 0x03:
        ramSize = 0x8000;
        ramBanks = 4;
        break;
    case 0x04:
        ramSize = 0x20000;
        ramBanks = 16;
        break;
    case 0x05:
        ramSize = 0x10000;
        ramBanks = 8;
        break;
    }
}

/**
 *  Gets ROM version from byte 0x014C; Usually 0x00
 */
void ROM::getROMVersion() { romVersion = rom[0x014C]; }

/**
 *  Reads the header checksum from byte 0x014D
 */
void ROM::getHeaderChecksum() { headerChecksum = rom[0x014D]; }

/**
 *  Reads the global checksum from bytes 0x014E, 0x014F
 */
void ROM::getGlobalChecksum() { globalChecksum = (rom[0x014E] << 0x8) | rom[0x014F]; }