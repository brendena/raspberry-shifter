#pragma once

#include "../usb/hid_keyboard.h"


/*
This to be the place where i convert 
ps2 into usb 
*/

void handle_ps2_keyboard_event(unsigned char ps2Key);
const USB_KeyboardState  getPs2KeyboardState();