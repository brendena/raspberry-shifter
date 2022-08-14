#include "hid_keyboard.h"
//--------------------------------------------------------------------+
// Keyboard
//--------------------------------------------------------------------+

LockingKeys lockingKeys ={0};
USB_KeyboardState usbKeyboardState;

static uint8_t const keycode2ascii[128][2] =  { HID_KEYCODE_TO_ASCII };


const USB_KeyboardState * getUSBKeyboardState() {return &usbKeyboardState;}

void handleKeyboardLed(uint8_t dev_addr, uint8_t instance,hid_keyboard_report_t const* report)
{
  static unsigned char status = 0; 
  unsigned char tmpStatus = status;

  unsigned char key = 0;
  unsigned char mod = 0; 
  for(uint8_t i=0; i<6; i++)
  {
    key = report->keycode[i];
    mod = 0;
    switch(key){
      case HID_KEY_CAPS_LOCK:
        printf("got here\n");
        mod = KEYBOARD_LED_CAPSLOCK;
        lockingKeys.capsLock = !lockingKeys.capsLock;
        break;
      case HID_KEY_NUM_LOCK:
        mod = KEYBOARD_LED_NUMLOCK;
        lockingKeys.numLock = !lockingKeys.numLock;
        break;
      case HID_KEY_SCROLL_LOCK:
        mod = KEYBOARD_LED_SCROLLLOCK;
        lockingKeys.scrollLock = !lockingKeys.scrollLock;
        break;
      default:
        break;
    }
    tmpStatus ^= mod; //invert the bit the selected keypress 
  }
  if(status != tmpStatus)
  {
    printf("got this working!\n");
    status = tmpStatus;
    tuh_hid_set_report(dev_addr,instance,0,HID_REPORT_TYPE_OUTPUT,&status,1);
  }
}



/*
// look up new key in previous keys
inline bool find_key_in_report(hid_keyboard_report_t const *report, uint8_t keycode)
{
  for(uint8_t i=0; i<6; i++)
  {
    if (report->keycode[i] == keycode)  return true;
  }

  return false;
}
*/

void process_kbd_report(hid_keyboard_report_t const *report)
{
  printf("test\n");
  memcpy(&usbKeyboardState.input, report, sizeof(hid_keyboard_report_t));
  usbKeyboardState.changed++; 
}


unsigned char checkAndResetChagnedUsb()
{
  unsigned char tmp = usbKeyboardState.changed;   
  usbKeyboardState.changed = 0;
  return tmp;
}