#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "bsp/board.h"
#include "tusb.h"
#include "usb/hid_keyboard.h"
#include "ps2/atkPico/atkPico.h"
#include "ps2ToUsb.h"

//--------------------------------------------------------------------+
// MACRO CONSTANT TYPEDEF PROTYPES
//--------------------------------------------------------------------+
void led_blinking_task(void);

extern void cdc_task(void);
extern void hid_app_task(void);

/*------------- MAIN -------------*/
int main(void)
{
  board_init();

  printf("[Raspberry-Shifter]\r\n");

  tusb_init();

  atk_init(1, 14);

  while (1)
  {
    // tinyusb host task
    // tuh_task();
    led_blinking_task();
    hid_app_task();
  }

  return 0;
}



//--------------------------------------------------------------------+
// TinyUSB Callbacks
//--------------------------------------------------------------------+

//--------------------------------------------------------------------+
// Blinking Task
//--------------------------------------------------------------------+
void led_blinking_task(void)
{
  const uint32_t interval_ms = 1000;
  static uint32_t start_ms = 0;

  static bool led_state = false;

  // Blink every interval ms
  if ( board_millis() - start_ms < interval_ms) return; // not enough time
  start_ms += interval_ms;

  board_led_write(led_state);
  led_state = 1 - led_state; // toggle
}


void hid_app_task(void)
{
  static hid_keyboard_report_t prev_report = { 0, 0, {0} }; // previous report to check key released

  const USB_KeyboardState keyboardState = getPs2KeyboardState();
  for(uint8_t i=0; i<6; i++)
  {
    if ( keyboardState.input.keycode[i] )
    {
      if ( find_key_in_report(&prev_report, keyboardState.input.keycode[i]) )
      {
        // exist in previous report means the current key is holding
      }else
      {
        // not existed in previous report means the current key is pressed
        bool const is_shift = keyboardState.input.modifier & (KEYBOARD_MODIFIER_LEFTSHIFT | KEYBOARD_MODIFIER_RIGHTSHIFT);
        uint8_t ch = keycode2ascii[keyboardState.input.keycode[i]][is_shift ? 1 : 0];
        putchar(ch);
        if ( ch == '\r' ) putchar('\n'); // added new line for enter key

        fflush(stdout); // flush right away, else nanolib will wait for newline
      }
    }
    // TODO example skips key released
  }

  prev_report = keyboardState.input;
}
