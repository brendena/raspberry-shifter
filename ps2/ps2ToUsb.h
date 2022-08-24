#pragma once

#include "../usb/hid_keyboard.h"


/*
This to be the place where i convert 
ps2 into usb 
*/

#define PS2_SHIFT_L      0x12
#define PS2_SHIFT_R      0x59
#define PS2_CONTROL_L    0x14
#define PS2_CONTROL_R    0x14 //0xe0
#define PS2_ALT_L        0x11
#define PS2_ALT_R        0x11 //0xe0
#define PS2_GUI_L        0x1f //0xe0
#define PS2_GUI_R        0x27 //oxe0


#define PS2_EXTENSION_FLAG 0xe0
#define PS2_RELASE_FLAG    0xf0



const USB_KeyboardState  getPs2KeyboardState();