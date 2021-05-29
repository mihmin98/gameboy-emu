#include "ROM.hpp"

ROM::ROM()
{
    rom = nullptr;
    ram = nullptr;

    ramEnable = false;
    currentROMBank = 1;
    currentRAMBank = 0;
    bankMode = 0;
    isMulticart = false;

    bootromActive = false;

    rtcS = 0;
    rtcM = 0;
    rtcH = 0;
    rtcDL = 0;
    rtcDH = 0;
    rtcLatchClockLastWritten = 0;
    rtcLatch = false;
    rtcLatchedSeconds = 0;
    rtcNumCycles = 0;
}

ROM::~ROM()
{
    if (rom != nullptr) {
        delete[] rom;
        rom = nullptr;
    }

    if (ram != nullptr) {
        delete[] ram;
        ram = nullptr;
    }

    if (saveFile != NULL) {
        fclose(saveFile);
    }
}

/**
 *  Loads the ROM at the specified path. Returns true on success
 */
bool ROM::loadROM(std::string romPath)
{
    romFilePath = romPath;
    saveFilePath = romPath;
    saveFilePath.replace_extension(".sav");

    FILE *f = fopen(romPath.c_str(), "rb");

    if (f != NULL) {
        fseek(f, 0, SEEK_END);
        romFileSize = ftell(f);
        fseek(f, 0, SEEK_SET);

        // Allocate mem for ROM
        try {
            rom = new uint8_t[romFileSize];
        } catch (const std::bad_alloc &e) {
            std::cerr << "ERROR: Could not allocate memory for ROM: " << e.what() << "\n";
            return false;
        }

        fread(rom, sizeof(uint8_t), romFileSize, f);
        fclose(f);

        // After the ROM has been loaded, read the header and run the checks
        readHeader();

        nintendoLogoOk = checkNintendoLogo();
        headerChecksumOk = checkHeaderChecksum();
        globalchecksumOk = checkGlobalChecksum();

        // Allocate cartridge RAM if necessary
        if (cartridgeRam || mbc == MBC::MBC2)
            ram = new uint8_t[ramSize];

        // Load save file
        loadSaveFile(saveFilePath);

        return true;

    } else {
        std::cerr << "ERROR: ROM " << romPath << " could not be opened\n";
        return false;
    }
}

bool ROM::loadSaveFile(fs::path savePath)
{
    if ((cartridgeRam || mbc == MBC::MBC2) && cartridgeBattery) {
        if (fs::exists(savePath)) {
            // Load the data in ram
            saveFile = fopen(savePath.c_str(), "r+b");
            if (saveFile == NULL) {
                std::cerr << "ERROR: Save file " << savePath.string() << " could not be opened\n";
                return false;
            }

            fseek(saveFile, 0, SEEK_END);
            size_t saveFileSize = ftell(saveFile);
            fseek(saveFile, 0, SEEK_SET);

            if (mbc == MBC3) {
                // Read rtc registers and increment time if necessary
                fread(&rtcS, sizeof(uint8_t), 1, saveFile);
                fread(&rtcM, sizeof(uint8_t), 1, saveFile);
                fread(&rtcH, sizeof(uint8_t), 1, saveFile);
                fread(&rtcDL, sizeof(uint8_t), 1, saveFile);
                fread(&rtcDH, sizeof(uint8_t), 1, saveFile);
                fread(&rtcLatchClockLastWritten, sizeof(uint8_t), 1, saveFile);
                fread(&rtcLatch, sizeof(bool), 1, saveFile);
                fread(&rtcLatchedSeconds, sizeof(uint64_t), 1, saveFile);
                fread(&rtcNumCycles, sizeof(uint16_t), 1, saveFile);

                time_t oldTime;
                fread(&oldTime, sizeof(time_t), 1, saveFile);

                time_t currentTime = time(NULL);

                if ((rtcDH & 0x40) == 0) {
                    incrementRtc(currentTime - oldTime);
                }
            }

            fread(ram, sizeof(uint8_t), ramSize, saveFile);

            return true;

        } else {
            // Create the file
            saveFile = fopen(savePath.c_str(), "w+b");
            if (saveFile == NULL) {
                std::cerr << "ERROR: Save file " << savePath.string() << " could not be created\n";
                return false;
            }

            return true;
        }
    }

    return false;
}

bool ROM::saveRam()
{
    if (saveFile == NULL) {
        return false;
    }

    fseek(saveFile, 0, SEEK_SET);

    if (mbc == MBC3) {
        // write rtc registers and current time
        fwrite(&rtcS, sizeof(uint8_t), 1, saveFile);
        fwrite(&rtcM, sizeof(uint8_t), 1, saveFile);
        fwrite(&rtcH, sizeof(uint8_t), 1, saveFile);
        fwrite(&rtcDL, sizeof(uint8_t), 1, saveFile);
        fwrite(&rtcDH, sizeof(uint8_t), 1, saveFile);
        fwrite(&rtcLatchClockLastWritten, sizeof(uint8_t), 1, saveFile);
        fwrite(&rtcLatch, sizeof(bool), 1, saveFile);
        fwrite(&rtcLatchedSeconds, sizeof(uint64_t), 1, saveFile);
        fwrite(&rtcNumCycles, sizeof(uint16_t), 1, saveFile);

        time_t currentTime = time(NULL);
        fwrite(&currentTime, sizeof(time_t), 1, saveFile);
    }

    if (fwrite(ram, sizeof(uint8_t), ramSize, saveFile) == 0) {
        return false;
    }

    return true;
}

bool ROM::loadBootrom(std::string bootromPath)
{
    FILE *f = fopen(bootromPath.c_str(), "rb");

    if (f == NULL) {
        std::cerr << "ERROR: Bootorm " << bootromPath << " could not be opened\n";
        return false;
    }

    fread(bootrom, sizeof(uint8_t), 256, f);

    fclose(f);

    bootromActive = true;
    return true;
}

void ROM::disableBootrom() { bootromActive = false; }

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
    if (memcmp(rom + 0x0104, goodNintendoLogo, 48 * sizeof(uint8_t)) == 0)
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
    uint8_t sum = 0;
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
        cartridgeRam = true;
        break;
    case 0x06:
        // MBC2 + BATTERY
        mbc = MBC::MBC2;
        cartridgeRam = true;
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
        if (mbc == MBC::MBC2) {
            // MBC2 cartrdige says it has no RAM, but it actually has 512*4 bits RAM
            ramSize = 512;
            ramBanks = 1;
        } else {
            ramSize = 0;
            ramBanks = 0;
        }
        break;
    case 0x01:
        ramSize = 0x800;
        ramBanks = 1;
        break;
    case 0x02:
        ramSize = 0x2000;
        ramBanks = 1;
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

/* CARTRIDGE READ AND WRITE */

/**
 *  Wrapper for calling the read on specific MBC
 */
uint8_t ROM::readmem(uint16_t addr)
{
    // There is no ROM loaded
    if (addr < 0x8000 && rom == nullptr)
        return 0xFF;

    // There is no RAM or RAM is not enabled
    if (addr >= 0xA000 && addr < 0xC000 && (!cartridgeRam || !ramEnable)) {
        return 0xFF;
    }

    if (bootromActive && addr < 0x100) {
        return bootrom[addr];
    }

    switch (mbc) {
    case MBC::None:
        return readmemNoMBC(addr);
        break;
    case MBC::MBC1:
        return readmemMBC1(addr);
        break;
    case MBC::MBC2:
        return readmemMBC2(addr);
        break;
    case MBC::MBC3:
        return readmemMBC3(addr);
        break;
    case MBC::MBC5:
        return readmemMBC5(addr);
        break;
    default:
        return 0xFF;
        break;
    }
}

/**
 * Wrapper for calling the write on specific MBC
 */
void ROM::writemem(uint8_t val, uint16_t addr)
{
    // Do nothing if there is no RAM or RAM is not enabled
    if (addr >= 0xA000 && addr < 0xC000 && (!cartridgeRam || !ramEnable)) {
        return;
    }

    switch (mbc) {
    case MBC::None:
        writememNoMBC(val, addr);
        break;
    case MBC::MBC1:
        writememMBC1(val, addr);
        break;
    case MBC::MBC2:
        writememMBC2(val, addr);
        break;
    case MBC::MBC3:
        writememMBC3(val, addr);
        break;
    case MBC::MBC5:
        writememMBC5(val, addr);
        break;
    default:
        return;
        break;
    }
}

uint8_t ROM::readmemNoMBC(uint16_t addr)
{
    // Read the ROM portion
    if (addr < 0x8000)
        return rom[addr];

    // Read the RAM portion
    if (addr >= 0xA000 && addr < 0xC000)
        return ram[addr - 0xA000];

    // Invalid address
    fprintf(stderr, "WARNING: Trying to read from invalid ROM address: 0x%04X\n", addr);
    return 0xFF;
}

void ROM::writememNoMBC(uint8_t val, uint16_t addr)
{
    // Set RAM enable
    if (addr < 0x2000) {
        ramEnable = (val & 0x0F) == 0x0A;
    }

    // Write to RAM
    if (addr >= 0xA000 && addr < 0xC000) {
        ram[addr - 0xA000] = val;
    }
}

uint8_t ROM::readmemMBC1(uint16_t addr)
{
    // ROM bank 0x00/0x20/0x40/0x60
    if (addr < 0x4000) {
        if (bankMode == 0) {
            // Bank 0x00
            return rom[addr];
        } else {
            // Bank 0x20/0x40/0x60
            // depending on bank size, not all bits are used
            uint8_t mask;
            if (romBanks > 96) {
                mask = 0x60;
            } else if (romBanks > 64 && romBanks <= 96) {
                mask = 0x40;
            } else if (romBanks > 32 && romBanks <= 64) {
                mask = 0x20;
            } else {
                mask = 0;
            }

            uint8_t actualBank = (currentRAMBank << 5) & mask;
            uint32_t actualAddr = addr + 0x4000 * actualBank;
            return rom[actualAddr];
        }
    }

    // Switchable ROM Bank
    if (addr >= 0x4000 && addr < 0x8000) {
        uint8_t mask = romBanks - 1;
        uint8_t actualBank = ((currentRAMBank << 5) | currentROMBank) & mask;
        uint32_t actualAddr = (addr - 0x4000) + 0x4000 * actualBank;

        return rom[actualAddr];
    }

    // RAM
    if (addr >= 0xA000 && addr < 0xC000) {
        if (bankMode == 0) {
            // ROM Bank Mode
            return ram[addr - 0xA000];
        } else {
            // RAM Bank Mode
            uint8_t mask = ramBanks - 1;
            uint32_t actualAddr = (addr - 0xA000) + 0x2000 * (currentRAMBank & mask);
            return ram[actualAddr];
        }
    }

    // Invalid address
    fprintf(stderr, "WARNING: Trying to read from invalid ROM address: 0x%04X\n", addr);
    return 0xFF;
}

void ROM::writememMBC1(uint8_t val, uint16_t addr)
{
    // RAM Enable
    if (addr < 0x2000) {
        ramEnable = (val & 0x0F) == 0x0A;
    }

    // ROM Bank Number
    if (addr >= 0x2000 && addr < 0x4000) {
        currentROMBank = val & 0x1F;
        if (currentROMBank == 0x00)
            currentROMBank = 0x01;
    }

    // RAM Bank or Secondary ROM Bank
    if (addr >= 0x4000 && addr < 0x6000) {
        currentRAMBank = val & 0x3;
    }

    // Bank Mode
    if (addr >= 0x6000 && addr < 0x8000) {
        bankMode = val & 1;
    }

    // RAM
    if (addr >= 0xA000 && addr < 0xC000) {
        if (bankMode == 0) {
            // ROM Bank Mode
            ram[addr - 0xA000] = val;
        } else {
            // RAM Bank Mode
            uint8_t mask = ramBanks - 1;
            uint32_t actualAddr = (addr - 0xA000) + 0x2000 * (currentRAMBank & mask);
            ram[actualAddr] = val;
        }
    }
}

uint8_t ROM::readmemMBC2(uint16_t addr)
{
    // ROM Bank 0
    if (addr < 0x4000)
        return rom[addr];

    // Switchable ROM Bank
    if (addr >= 0x4000 && addr < 0x8000) {
        uint8_t mask = romBanks - 1;
        uint32_t actualAddr = (addr - 0x4000) + 0x4000 * (currentROMBank & mask);
        return rom[actualAddr];
    }

    // RAM
    if (addr >= 0xA000 && addr < 0xC000) {
        addr = (addr - 0xA000) & 0x1FF;
        return ((ram[addr] & 0x0F) | 0xF0);
    }

    // Invalid address
    fprintf(stderr, "WARNING: Trying to read from invalid ROM address: 0x%04X\n", addr);
    return 0xFF;
}

void ROM::writememMBC2(uint8_t val, uint16_t addr)
{
    // RAM Enable
    if (addr < 0x4000 && (addr & 0x100) == 0) {
        ramEnable = (val & 0xF) == 0xA;
    }

    // ROM Bank
    if (addr < 0x4000 && (addr & 0x100) == 0x100) {
        currentROMBank = val /*& 0xF*/;
        if ((currentROMBank & 0xF) == 0)
            currentROMBank = 1;
    }

    // RAM
    if (addr >= 0xA000 && addr < 0xC000) {
        addr = (addr - 0xA000) & 0x1FF;
        ram[addr] = val & 0xF;
    }
}

uint8_t ROM::readmemMBC3(uint16_t addr)
{
    // ROM Bank 0
    if (addr < 0x4000)
        return rom[addr];

    // Switchable ROM Bank
    if (addr >= 0x4000 && addr < 0x8000) {
        uint32_t acutalAddr = (addr - 0x4000) + 0x4000 * currentROMBank;
        return rom[acutalAddr];
    }

    // RAM
    if (addr >= 0xA000 && addr < 0xC000 && currentRAMBank < 0x4) {
        uint32_t actualAddr = (addr - 0xA000) + 0x2000 * currentRAMBank;
        return ram[actualAddr];
    }

    // RTC Register
    if (addr >= 0xA000 && addr < 0xC000 && currentRAMBank >= 0x08 && currentRAMBank < 0x0D) {
        switch (currentRAMBank) {
        case 0x08:
            return rtcS;
            break;
        case 0x09:
            return rtcM;
            break;
        case 0x0A:
            return rtcH;
            break;
        case 0x0B:
            return rtcDL;
            break;
        case 0x0C:
            return rtcDH;
            break;
        }
    }

    // Invalid address
    fprintf(stderr, "WARNING: Trying to read from invalid ROM address: 0x%04X\n", addr);
    return 0xFF;
}

void ROM::writememMBC3(uint8_t val, uint16_t addr)
{
    // RAM / Timer Enable
    if (addr < 0x2000) {
        ramEnable = (val & 0x0A) == 0x0A;
    }

    // ROM Bank Number
    if (addr >= 0x2000 && addr < 0x4000) {
        currentROMBank = val;
        if (currentROMBank == 0)
            currentROMBank = 1;
    }

    // RAM Bank Number / RTC Register Select
    if (addr >= 0x4000 && addr < 0x6000) {
        currentRAMBank = val;
    }

    // Latch Clock Data
    if (addr >= 0x6000 && addr < 0x8000) {
        if (val == 0x01 && rtcLatchClockLastWritten == 0x00) {
            // Toggle Latch
            rtcLatch = !rtcLatch;

            if (rtcLatch == false) {
                // Latch reset
                incrementRtc(rtcLatchedSeconds);
                rtcLatchedSeconds = 0;
            } else if (rtcLatch) {
                // Latch set
                rtcLatchedSeconds = 0;
            }
        }
        rtcLatchClockLastWritten = val;
    }

    // RAM
    if (addr >= 0xA000 && addr < 0xC000 && currentRAMBank < 0x4) {
        uint32_t actualAddr = (addr - 0xA000) + 0x2000 * currentRAMBank;
        ram[actualAddr] = val;
    }

    // RTC Registers
    if (addr >= 0xA000 && addr < 0xC000 && currentRAMBank >= 0x08 && currentRAMBank < 0x0D) {
        switch (currentRAMBank) {
        case 0x08:
            rtcS = val;
            break;
        case 0x09:
            rtcM = val;
            break;
        case 0x0A:
            rtcH = val;
            break;
        case 0x0B:
            rtcDL = val;
            break;
        case 0x0C:
            rtcDH = val;
            break;
        }
    }
}

uint8_t ROM::readmemMBC5(uint16_t addr)
{
    // ROM Bank 0
    if (addr < 0x4000)
        return rom[addr];

    // Switchable ROM Bank
    if (addr >= 0x4000 && addr < 0x8000) {
        uint8_t mask = romBanks - 1;
        uint32_t actualAddr = (addr - 0x4000) + 0x4000 * (currentROMBank & mask);
        return rom[actualAddr];
    }

    // RAM
    if (addr >= 0xA000 && addr < 0xC000) {
        uint32_t acutalAddr = (addr - 0xA000) + 0x2000 * currentRAMBank;
        return ram[acutalAddr];
    }

    // Invalid address
    fprintf(stderr, "WARNING: Trying to read from invalid ROM address: 0x%04X\n", addr);
    return 0xFF;
}

void ROM::writememMBC5(uint8_t val, uint16_t addr)
{
    // RAM Enable
    if (addr < 0x2000) {
        ramEnable = (val & 0xFF) == 0x0A;
    }

    // Low 8 bits of ROM Bank number
    if (addr >= 0x2000 && addr < 0x3000) {
        currentROMBank = (currentROMBank & 0x0100) | val;
    }

    // High bit (bit 9) of ROM Bank number
    if (addr >= 0x3000 && addr < 0x4000) {
        currentROMBank = ((val & 0x1) << 8) | (currentROMBank & 0xFF);
    }

    // RAM Bank Number
    if (addr >= 0x4000 && addr < 0x6000) {
        currentRAMBank = val & 0xF;
    }

    // RAM
    if (addr >= 0xA000 && addr < 0xC000) {
        uint32_t acutalAddr = (addr - 0xA000) + 0x2000 * currentRAMBank;
        ram[acutalAddr] = val;
    }
}

void ROM::cycleRtc()
{
    if ((rtcDH & 0x40) != 0) {
        // Do not increment if halted
        return;
    }

    ++rtcNumCycles;

    if (rtcNumCycles == ROM_RTC_TICKS_UNTIL_INCREMENT) {
        rtcNumCycles = 0;

        if (rtcLatch) {
            ++rtcLatchedSeconds;
        } else {
            incrementRtc(1);
        }
    }
}

void ROM::incrementRtc(uint64_t numSeconds)
{
    uint64_t oldTime = rtcS + rtcM * 60 + rtcH * 3600 + (((rtcDH & 1) << 8) | rtcDL) * 86400;
    uint64_t newTime = oldTime + numSeconds;

    uint16_t numDays = newTime / 86400;
    if (numDays > 0x1FF) {
        // set carry bit
        rtcDH |= 0x80;
    }

    rtcDH = (rtcDH & 0xC0) | ((numDays & 0x100) >> 8);
    rtcDL = numDays & 0xFF;

    newTime -= numDays * 86400;

    uint8_t numHours = newTime / 3600;
    rtcH = numHours;

    newTime -= numHours * 3600;

    uint8_t numMinutes = newTime / 60;
    rtcM = numMinutes;

    newTime -= numMinutes * 60;

    rtcS = newTime;
}
