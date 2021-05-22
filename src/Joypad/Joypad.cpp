#include "Joypad.hpp"
#include "Memory.hpp"
#include "SM83.hpp"

Joypad::Joypad()
{
    for (uint i = 0; i < 4; ++i) {
        keyState[0][i] = false;
        keyState[1][i] = false;
    }
}

void Joypad::cycle()
{
    uint8_t joypadRegister = memory->readmem(0xFF00, true, true);
    bool directionButtonsEnabled = !(joypadRegister & 0x10);
    bool actionButtonsEnabled = !(joypadRegister & 0x20);
    uint8_t buttonState;

    // // Direction Buttons
    // for (uint8_t i = 0; i < 4; ++i) {
    //     buttonState = joypadRegister & (1 << i);
    //     mask = 0xFF - (1 << i);
    //     if (keyState[0][i] == 1) {
    //         if (directionButtonsEnabled) {
    //             // button pressed and enabled
    //             if (buttonState == 1) {
    //                 cpu->setInterruptFlag(1);
    //             }

    //             joypadRegister &= mask;
    //         } else {
    //             // button pressed and not enabled
    //             joypadRegister = (joypadRegister & mask) | (1 << i);
    //         }
    //     } else {
    //         // button not pressed
    //         joypadRegister = (joypadRegister & mask) | (1 << i);
    //     }
    // }

    //     // Action Buttons
    // for (uint8_t i = 0; i < 4; ++i) {
    //     buttonState = joypadRegister & (1 << i);
    //     mask = 0xFF - (1 << i);
    //     if (keyState[1][i] == 1) {
    //         if (actionButtonsEnabled) {
    //             // button pressed and enabled
    //             if (buttonState == 1) {
    //                 cpu->setInterruptFlag(1);
    //             }

    //             joypadRegister &= mask;
    //         } else {
    //             // button pressed and not enabled
    //             joypadRegister = (joypadRegister & mask) | (1 << i);
    //         }
    //     } else {
    //         // button not pressed
    //         // if action buttons are enabled then overwrite, if disabled then do nothing?
    //         if (!directionButtonsEnabled && !keyState[0][i])
    //             joypadRegister = (joypadRegister & mask) | (1 << i);
    //     }
    // }

    // Direction Buttons
    // Right Button
    buttonState = joypadRegister & 0x1;
    if (keyState[0][0] == 1) {
        if (directionButtonsEnabled) {
            // button pressed and enabled
            if (buttonState == 1) {
                cpu->setJoypadInterruptFlag(1);
            }

            joypadRegister = joypadRegister & 0xFE;
        } else {
            // button pressed and not enabled
            joypadRegister = (joypadRegister & 0xFE) | 0x1;
        }
    } else {
        // button is not pressed, set to 1
        joypadRegister = (joypadRegister & 0xFE) | 0x1;
    }

    // Left Button
    buttonState = joypadRegister & 0x2;
    if (keyState[0][1] == 1) {
        if (directionButtonsEnabled) {
            // button pressed and enabled
            if (buttonState == 1) {
                cpu->setJoypadInterruptFlag(1);
            }

            joypadRegister = joypadRegister & 0xFD;
        } else {
            // button pressed and not enabled
            joypadRegister = (joypadRegister & 0xFD) | 0x2;
        }
    } else {
        // button is not pressed, set to 1
        joypadRegister = (joypadRegister & 0xFD) | 0x2;
    }

    // Up Button
    buttonState = joypadRegister & 0x4;
    if (keyState[0][2] == 1) {
        if (directionButtonsEnabled) {
            // button pressed and enabled
            if (buttonState == 1) {
                cpu->setJoypadInterruptFlag(1);
            }

            joypadRegister = joypadRegister & 0xFB;
        } else {
            // button pressed and not enabled
            joypadRegister = (joypadRegister & 0xFB) | 0x4;
        }
    } else {
        // button is not pressed, set to 1
        joypadRegister = (joypadRegister & 0xFB) | 0x4;
    }

    // Down Button
    buttonState = joypadRegister & 0x8;
    if (keyState[0][3] == 1) {
        if (directionButtonsEnabled) {
            // button pressed and enabled
            if (buttonState == 1) {
                cpu->setJoypadInterruptFlag(1);
            }

            joypadRegister = joypadRegister & 0xF7;
        } else {
            // button pressed and not enabled
            joypadRegister = (joypadRegister & 0xF7) | 0x8;
        }
    } else {
        // button is not pressed, set to 1
        joypadRegister = (joypadRegister & 0xF7) | 0x8;
    }

    // Action Buttons
    // Be careful to not overwrite state if direction is enabled and pressed when action not
    // enabled
    // When action is enabled weird behaviour

    // A Button
    buttonState = joypadRegister & 0x1;
    if (keyState[1][0] == 1) {
        if (actionButtonsEnabled) {
            // button pressed and enabled
            if (buttonState == 1) {
                cpu->setJoypadInterruptFlag(1);
            }

            joypadRegister = joypadRegister & 0xFE;
        } else {
            // button pressed and not enabled
            joypadRegister = (joypadRegister & 0xFE) | 0x1;
        }
    } else {
        // button is not pressed, set to 1, if direction is not enabled and pressed
        if (!directionButtonsEnabled && !keyState[0][0])
            joypadRegister = (joypadRegister & 0xFE) | 0x1;
    }

    // B Button
    buttonState = joypadRegister & 0x2;
    if (keyState[1][1] == 1) {
        if (actionButtonsEnabled) {
            // button pressed and enabled
            if (buttonState == 1) {
                cpu->setJoypadInterruptFlag(1);
            }

            joypadRegister = joypadRegister & 0xFD;
        } else {
            // button pressed and not enabled
            joypadRegister = (joypadRegister & 0xFD) | 0x2;
        }
    } else {
        // button is not pressed, set to 1, if direction is not enabled and pressed
        if (!directionButtonsEnabled && !keyState[0][1])
            joypadRegister = (joypadRegister & 0xFD) | 0x2;
    }

    // Select Button
    buttonState = joypadRegister & 0x4;
    if (keyState[1][2] == 1) {
        if (actionButtonsEnabled) {
            // button pressed and enabled
            if (buttonState == 1) {
                cpu->setJoypadInterruptFlag(1);
            }

            joypadRegister = joypadRegister & 0xFB;
        } else {
            // button pressed and not enabled
            joypadRegister = (joypadRegister & 0xFB) | 0x4;
        }
    } else {
        // button is not pressed, set to 1, if direction is not enabled and pressed
        if (!directionButtonsEnabled && !keyState[0][2])
            joypadRegister = (joypadRegister & 0xFB) | 0x4;
    }

    // Start Button
    buttonState = joypadRegister & 0x8;
    if (keyState[1][3] == 1) {
        if (actionButtonsEnabled) {
            // button pressed and enabled
            if (buttonState == 1) {
                cpu->setJoypadInterruptFlag(1);
            }

            joypadRegister = joypadRegister & 0xF7;
        } else {
            // button pressed and not enabled
            joypadRegister = (joypadRegister & 0xF7) | 0x8;
        }
    } else {
        // button is not pressed, set to 1, if direction is not enabled and pressed
        if (!directionButtonsEnabled && !keyState[0][3])
            joypadRegister = (joypadRegister & 0xF7) | 0x8;
    }

    // Write joypad register
    memory->writemem(joypadRegister, 0xFF00, true, true);
}
