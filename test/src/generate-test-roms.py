#!/usr/bin/env python3

from os import path
import argparse
from pathlib import Path


def generate_nintendo_logo():
    return [0xCE, 0xED, 0x66, 0x66, 0xCC, 0x0D, 0x00, 0x0B, 0x03, 0x73,
            0x00, 0x83, 0x00, 0x0C, 0x00, 0x0D, 0x00, 0x08, 0x11, 0x1F,
            0x88, 0x89, 0x00, 0x0E, 0xDC, 0xCC, 0x6E, 0xE6, 0xDD, 0xDD,
            0xD9, 0x99, 0xBB, 0xBB, 0x67, 0x63, 0x6E, 0x0E, 0xEC, 0xCC,
            0xDD, 0xDC, 0x99, 0x9F, 0xBB, 0xB9, 0x33, 0x3E]


def generate_cartridge(file_name, good_nintendo_logo=True, game_title='GAME TITLE', gbc_flag=0x80,
                       sgb_flag=0x00, cartridge_type=0x00, rom_size=0x00, ram_size=0x00,
                       rom_version=0x00, header_checksum=0x00, global_checksum=0x00):

    actual_rom_size = (0x8000 << rom_size)
    rom = [0x00] * actual_rom_size

    # Nintendo logo
    if good_nintendo_logo:
        nintendo_logo = generate_nintendo_logo()
        for i, val in enumerate(nintendo_logo):
            rom[0x0104 + i] = val

    # Game Title
    for i, val in enumerate(game_title):
        rom[0x0134 + i] = ord(val)

    # GBC Flag
    rom[0x0143] = gbc_flag

    # SGB Flag
    rom[0x0146] = sgb_flag

    # Cartridge Type
    rom[0x0147] = cartridge_type

    # ROM Size
    rom[0x0148] = rom_size

    # RAM Size
    rom[0x0149] = ram_size

    # ROM Version
    rom[0x014C] = rom_version

    # Header Checksum
    if header_checksum == 0x00:
        # Calculate corret checksum
        good_header_checksum = 0
        i = 0x0134
        while i <= 0x014C:
            good_header_checksum = (good_header_checksum - rom[i] - 1) % 256
            i += 1
        rom[0x014D] = good_header_checksum
    else:
        rom[0x014D] = header_checksum

    # Global Checksum
    if global_checksum == 0x0000:
        good_global_checksum = 0
        for i in range(0, 0x014E):
            good_global_checksum = (good_global_checksum + rom[i]) % 65536
        for i in range(0x0150, actual_rom_size):
            good_global_checksum = (good_global_checksum + rom[i]) % 65536
        rom[0x014E] = (good_global_checksum & 0xFF00) >> 0x8
        rom[0x014F] = good_global_checksum & 0x00FF
    else:
        rom[0x014E] = (global_checksum & 0xFF00) >> 0x8
        rom[0x014F] = global_checksum & 0x00FF

    # Write the file
    rom_bytes = bytes(rom)
    with open(file_name, 'wb') as f:
        f.write(rom_bytes)


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Generate test roms')
    parser.add_argument('-d', '--dir', default='.',
                        help='location where the rom files should be created')

    args = parser.parse_args()

    rom_folder_path = args.dir

    # Create folder
    Path(rom_folder_path).mkdir(parents=True, exist_ok=True)

    # Bad Ninendo Logo, Bad Header Checksum, Bad Global Checksum, GBC Only ROM
    rom_name = 'test_bad_nintendo_logo.gb'
    rom_path = path.join(rom_folder_path, rom_name)
    if path.exists(rom_path):
        print(rom_name + ' already exists, skipping...')
    else:
        generate_cartridge(rom_path, good_nintendo_logo=False,
                           game_title='BAD CHECKSUMS', gbc_flag=0xC0, header_checksum=0x01,
                           global_checksum=0x01)

    # NoMBC, No RAM
    rom_name = 'test_no_mbc_no_ram.gb'
    rom_path = path.join(rom_folder_path, rom_name)
    if path.exists(rom_path):
        print(rom_name + ' already exists, skipping...')
    else:
        generate_cartridge(rom_path,
                           game_title='NOMBC NORAM')

    # NoMBC, RAM
    rom_name = 'test_no_mbc_ram.gb'
    rom_path = path.join(rom_folder_path, rom_name)
    if path.exists(rom_path):
        print(rom_name + ' already exists, skipping...')
    else:
        generate_cartridge(rom_path, game_title='NOMBC RAM',
                           cartridge_type=0x08, ram_size=0x02)

    # MBC1
    rom_name = 'test_mbc1.gb'
    rom_path = path.join(rom_folder_path, rom_name)
    if path.exists(rom_path):
        print(rom_name + ' already exists, skipping...')
    else:
        generate_cartridge(rom_path, game_title='MBC1',
                           cartridge_type=0x02, rom_size=0x06, ram_size=0x03)

    # MBC2
    rom_name = 'test_mbc2.gb'
    rom_path = path.join(rom_folder_path, rom_name)
    if path.exists(rom_path):
        print(rom_name + ' already exists, skipping...')
    else:
        generate_cartridge(rom_path, game_title='MBC2',
                           cartridge_type=0x05, rom_size=0x03)

    # MBC3
    rom_name = 'test_mbc3.gb'
    rom_path = path.join(rom_folder_path, rom_name)
    if path.exists(rom_path):
        print(rom_name + ' already exists, skipping...')
    else:
        generate_cartridge(rom_path, game_title='MBC3',
                           cartridge_type=0x12, rom_size=0x06, ram_size=0x05)

    # MBC5
    rom_name = 'test_mbc5.gb'
    rom_path = path.join(rom_folder_path, rom_name)
    if path.exists(rom_path):
        print(rom_name + ' already exists, skipping...')
    else:
        generate_cartridge(rom_path, game_title='MBC5',
                           cartridge_type=0x1A, rom_size=0x08, ram_size=0x04)
