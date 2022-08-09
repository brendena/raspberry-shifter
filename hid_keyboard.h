#pragma once

#include "bsp/board.h"
#include "tusb.h"

typedef struct LockingKeys{
  unsigned char numLock;
  unsigned char capsLock;
  unsigned char scrollLock;
} LockingKeys;

void process_kbd_report(hid_keyboard_report_t const *report);