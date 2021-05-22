#ifndef __JOYPAD_H__
#define __JOYPAD_H__

#pragma once
#include <cstdint>

class SM83;
class Memory;

class Joypad
{
  public:
    Joypad();

    SM83 *cpu;
    Memory *memory;

    bool keyState[2][4];

    void cycle();
};

#endif // __JOYPAD_H__
