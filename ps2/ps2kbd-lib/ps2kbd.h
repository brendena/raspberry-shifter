/* Copyright (C) 1883 Thomas Edison - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the GPLv2 license, which unfortunately won't be
 * written for another century.
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */
#pragma once

#ifdef __cplusplus
extern "C" {
#endif


//The include the parity bit
enum PS2_COMMANDS
{
    //F0 might be get scan code???
    PS2_COMMANDS_RED_ID = 0x0F2 ,
    PS2_COMMANDS_SET_LEDS = 0x1ED
};


/*
Very common keys 
*/

#define PS2_SHIFT_L      0x12
#define PS2_SHIFT_R      0x59
#define PS2_CONTROL_L    0x14
#define PS2_EXT_CONTROL_R    0x14 //0xe0
#define PS2_ALT_L        0x11
#define PS2_EXT_ALT_R        0x11 //0xe0
#define PS2_EXT_GUI_L        0x1f //0xe0
#define PS2_EXT_GUI_R        0x27 //oxe0


//extension units
#define PS2_EXT_SYSTEM_POWER 0x37
#define PS2_EXT_SYSTEM_SLEEP 0x3F
#define PS2_EXT_SYSTEM_WAKE  0x5E
#define PS2_EXT_PRINT_SCREEN 0x7C
#define PS2_EXT_INSERT       0x70
#define PS2_EXT_HOME         0x6C
#define PS2_EXT_PAGE_UP      0x7D
#define PS2_EXT_DELETE       0x71
#define PS2_EXT_END          0x69
#define PS2_EXT_PAGE_DOWN    0x7A
#define PS2_EXT_RIGHT_ARROW  0x74
#define PS2_EXT_LEFT_ARROW   0x6B
#define PS2_EXT_DOWN_ARROW   0x72
#define PS2_EXT_UP_ARROW     0x75


/*
break/ctrl-pause
*/




//LOCKING_BUTTONS/LED VALUES
#define PS2_CAPS_LOCK    0x58
#define PS2_SCROLL_LOCK  0x7E
#define PS2_NUM_LOCK     0x77


#define PS2_EXTENSION_FLAG 0xe0
#define PS2_RELASE_FLAG    0xf0

#define PS2_ACK_RESPONSE 0xFA // 250


typedef struct Ps2LockingKeys
{
    unsigned char scrollLock : 1;
    unsigned char numLock : 1;
    unsigned char capsLock : 1;
}Ps2LockingKeys;

typedef union Ps2LockingKeysUnion
{
    Ps2LockingKeys keys;
    unsigned char value;
} Ps2LockingKeysUnion;


// Initialize PS/2 keyboard support
// Parameters
//     pio  - keyboard suport pio number. 0 or 1
//     gpio - GPIO number of data pin, ctl pin must be on next
//            adjacent GPIO
// Returns  - none
void kbd_init(uint pio, uint gpio);

// Return keyboard status
// Parameters - none
// Returns  - 0 for not ready, otherwise ready
int kbd_ready(void);

// Blocking keyboard read
// Parameters - none
// Returns  - single ASCII character
char kbd_getc(void);

void kbd_send_command(unsigned short data );

unsigned int kbd_add_parity(unsigned char byte);

#ifdef __cplusplus
}
#endif
