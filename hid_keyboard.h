#pragma once

#include "bsp/board.h"
#include "tusb.h"

/*
modifiing keys that hold there state
*/
typedef struct LockingKeys{
  unsigned char numLock : 1;
  unsigned char capsLock : 1;
  unsigned char scrollLock : 1;
  unsigned char compose : 1;
  unsigned char kana : 1;
} LockingKeys;

typedef struct USB_KeyboardState
{
  hid_keyboard_report_t input;
  LockingKeys lockedMod;
  unsigned char changed;
} USB_KeyboardState;

//used internally
void process_kbd_report(hid_keyboard_report_t const *report);
void handleKeyboardLed(uint8_t dev_addr, uint8_t instance,hid_keyboard_report_t const* report);


//externally
unsigned char checkAndResetChagnedUsb();
const USB_KeyboardState*  getUSBKeyboardState();