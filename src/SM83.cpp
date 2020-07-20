#include "SM83.hpp"

uint8_t SM83::getZeroFlag()
{
    return (F & 0x80) >> 0x7;
}

uint8_t SM83::getSubtractFlag()
{
    return (F & 0x40) >> 0x6;
}

uint8_t SM83::getHalfCarryFlag()
{
    return (F & 0x20) >> 0x5;
}

uint8_t SM83::getCarryFlag()
{
    return (F & 0x10) >> 0x4;
}

void SM83::setZeroFlag(uint8_t value)
{
    value = value != 0 ? 1 : 0;

    F |= value << 0x7;
}

void SM83::setSubtractFlag(uint8_t value)
{
    value = value != 0 ? 1 : 0;

    F |= value << 0x6;
}

void SM83::setHalfCarryFlag(uint8_t value)
{
    value = value != 0 ? 1 : 0;

    F |= value << 0x5;
}

void SM83::setCarryFlag(uint8_t value)
{
    value = value != 0 ? 1 : 0;

    F |= value << 0x4;
}

