#include "../usb/hid_keyboard.h"
#include "usbToPs2Mapping.h"
#include "ps2ToUsb.h"


/*
This to be the place where i convert 
ps2 into usb 
*/

LockingKeys ps2LockingKeys;
USB_KeyboardState ps2KeyboardState;

const USB_KeyboardState  getPs2KeyboardState(){
    return ps2KeyboardState;
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
    //printf(" [key] added\n");
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
    //printf(" [key] released\n");
}

void handle_ps2_keyboard_event(unsigned char ps2Key){
    static uint8_t release; // Flag indicates the release of a key
    static uint8_t extNum; // PS2_EXTENSION_NUM
    unsigned char usbKeyPress = ps2UsbMapping[ps2Key];
    unsigned char modifierPressed = 0;

    //
    if(!extNum)
    {
        switch (ps2Key) {
            case PS2_RELASE_FLAG:               // key-release code 0xF0 detected
                release = 1;         // set release
                break;               // go back to start
            case PS2_EXTENSION_FLAG:
                extNum = 1;
                break;
            case PS2_SHIFT_L:               // Left-side SHIFT key detected
                modifierPressed = KEYBOARD_MODIFIER_LEFTSHIFT;  
                break;
            case PS2_SHIFT_R:               // Right-side SHIFT key detected
                modifierPressed = KEYBOARD_MODIFIER_RIGHTSHIFT;  
                break;
            case PS2_CONTROL_L:               //Left Control
                modifierPressed = KEYBOARD_MODIFIER_LEFTCTRL;
                break;
            case PS2_ALT_L:               // Right-side SHIFT key detected
                modifierPressed = KEYBOARD_MODIFIER_LEFTALT;  
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
                extNum = 0;
                break;
        }
    }
    else{
        switch (ps2Key) {
            case PS2_RELASE_FLAG:
                release = 1;         
                break;               
            case PS2_CONTROL_R:               
                modifierPressed = KEYBOARD_MODIFIER_RIGHTCTRL;         
                break;               
            case PS2_ALT_R:
                modifierPressed = KEYBOARD_MODIFIER_RIGHTALT;  
                break;
            case PS2_GUI_L:               
                modifierPressed = KEYBOARD_MODIFIER_LEFTGUI;  
                break;
            case PS2_GUI_R:               
                modifierPressed = KEYBOARD_MODIFIER_RIGHTGUI;  
                break;
        }
    }




    if(modifierPressed)
    {
        if (release) {       
            ps2KeyboardState.input.modifier ^= modifierPressed;
            release = 0;     // Clear key-release flag
        } else {
            ps2KeyboardState.input.modifier |= modifierPressed; 
        }   
        release = 0;
        extNum = 0;
    }
    ps2KeyboardState.changed = 1;
}

