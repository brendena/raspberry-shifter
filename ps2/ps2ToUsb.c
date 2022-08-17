#include "../usb/hid_keyboard.h"
#include "usbToPs2Mapping.h"


/*
This to be the place where i convert 
ps2 into usb 
*/

LockingKeys ps2LockingKeys;
USB_KeyboardState ps2KeyboardState;

const USB_KeyboardState*  getPs2KeyboardState(){
    return &ps2KeyboardState;
}

void addKey(hid_keyboard_report_t *input, unsigned char usbKey)
{
    for(unsigned char i =0; i < 6; i++)
    {
        if(input->keycode[i] == 0 || input->keycode[i] == usbKey ){
            input->keycode[i] = usbKey;
            break;
        }
    }
}

void removeKey(hid_keyboard_report_t *input, unsigned char usbKey)
{
    unsigned char i =0;
    for(i; i < 6; i++)
    {
        if(input->keycode[i] == usbKey){
            input->keycode[i] = 0;
            break;
        }
    }
    for(i; i < 5; i++)
    {
        input->keycode[i] = input->keycode[i + 1];
    }
    input->keycode[5] = 0;
    printf("key released\n");
}

void handle_ps2_keyboard_event(unsigned char ps2Key){
    static uint8_t release; // Flag indicates the release of a key

    unsigned char usbKeyPress = ps2UsbMapping[ps2Key];

    switch (ps2Key) {
        case 0xF0:               // key-release code 0xF0 detected
            release = 1;         // set release
            break;               // go back to start
        case 0x12:               // Left-side SHIFT key detected
            if (release) {       
                ps2KeyboardState.input.modifier ^= KEYBOARD_MODIFIER_LEFTSHIFT;
                release = 0;     // Clear key-release flag
            } else {
                ps2KeyboardState.input.modifier |= KEYBOARD_MODIFIER_LEFTSHIFT; 
            }      
            break;
        case 0x59:               // Right-side SHIFT key detected
            if (release) {       
                ps2KeyboardState.input.modifier ^= KEYBOARD_MODIFIER_RIGHTSHIFT;
                release = 0;     // Clear key-release flag
            } else {
                ps2KeyboardState.input.modifier |= KEYBOARD_MODIFIER_RIGHTSHIFT; 
            }   
            break;
        default:
            // no case applies
            if (!release){                              // If no key-release detected yet
                addKey(&ps2KeyboardState.input,usbKeyPress);
            }
            else{
                removeKey(&ps2KeyboardState.input,usbKeyPress);
            }
            release = 0;
            break;
    }
    ps2KeyboardState.changed = 1;
}

