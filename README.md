# gameboy-emu
A GameBoy emulator written in C++ that uses SDL2

## Getting Started
### Prerequisites
- [SDL2](https://www.libsdl.org/index.php). To install SDL2, visit [this](https://wiki.libsdl.org/Installation) page.
- A C++ compiler with c++17 support
- Python3 for creating test roms

### Installing
Clone the repo and then use cmake and your preffered build system (Make, ninja, etc.)

For example, when using Unix Makefiles:
```
git clone https://github.com/mihmin98/gameboy-emu.git
cd gameboy-emu
mkdir build
cd build
cmake ..
make
```
The binary executable file is ``gameboy-emu`` and it is located in the `build` folder

### Running 
```
gameboy-emu rom_path [flags]

Flags:
        -g dmg | cgb: Selects gameboy mode: DMG or CGB. By default DMG is selected
        -w windowSize: How big should the window be compared to the gameboy's resolution of 160x144
        -b bootromPath: Path to the DMG bootrom
        -h: Prints this message
```

### Configuration
When running gameboy-emu for the first time it will create a `gameboy-emu.ini` file which can be used to configure certain parameters.
* Window Size: How big should the window be compared to the gameboy's resolution of 160x144
* Audio Batch Cycles: Increasing this value may lead to better performance, but can cause audio glitches
* Audio Volume
* Use Bootrom: Specifies if the bootrom should be run
* Bootrom Path: Path to the DMG bootrom
* Print Performance Info: Print performance info in the console

### Running Tests
Use `ctest` or the executable `unit_tests` to run the tests 

## Built with
* [SDL2](https://www.libsdl.org/index.php) - Graphics Library
* [Catch2](https://github.com/catchorg/Catch2) - Test Suite
* [inih](https://github.com/benhoyt/inih) - .INI Parser