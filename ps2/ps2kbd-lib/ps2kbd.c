/* Copyright (C) 1883 Thomas Edison - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the GPLv2 license, which unfortunately won't be
 * written for another century.
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */
#include "ps2kbd.pio.h"

#include "ps2kbd.h"

#include "hardware/clocks.h"
#include "hardware/pio.h"

static PIO kbd_pio;         // pio0 or pio1
static uint kbd_sm;         // pio state machine index
static uint base_gpio;      // data signal gpio #
static unsigned char kbd_key_buff; //holds the currently pressed key

void __attribute__((weak)) handle_ps2_keyboard_event(unsigned char ps2Key){}


void on_pio0_irq0(){
    pio_interrupt_get(kbd_pio, 0);
    pio_interrupt_clear(kbd_pio, 0);
    irq_clear(PIO1_IRQ_0);

    static uint8_t release; // Flag indicates the release of a key
    static uint8_t shift;   // Shift indication
    static uint8_t ascii;   // Translated to ASCII

    if (pio_sm_is_rx_fifo_empty(kbd_pio, kbd_sm))
        return; // no new codes in the fifo
    uint8_t code = *((io_rw_8*)&kbd_pio->rxf[kbd_sm] + 3);
    handle_ps2_keyboard_event(code);

    
}

void kbd_init(uint pio, uint gpio) {
    kbd_pio = pio ? pio1 : pio0;
    base_gpio = gpio; // base_gpio is data signal, base_gpio+1 is clock signal


    // with pull up
    gpio_pull_up(base_gpio);
    gpio_pull_up(base_gpio + 1);
    
    // get a state machine
    kbd_sm = pio_claim_unused_sm(kbd_pio, true);
    // reserve program space in SM memory
    uint offset = pio_add_program(kbd_pio, &ps2kbd_program);
    // program the start and wrap SM registers
    pio_sm_config c = ps2kbd_program_get_default_config(offset);

    sm_config_set_set_pins(&c, base_gpio + 1,1);
    sm_config_set_jmp_pin(&c, base_gpio + 1); // set the EXECCTRL_JMP_PIN

//configure out section PS2
    // Set pin directions base
    pio_sm_set_consecutive_pindirs(kbd_pio, kbd_sm, base_gpio + 1, 1, false);
    // Set the out pins
    sm_config_set_out_pins(&c, base_gpio + 1, 2);
// Shift 8 bits to the right, autopush enabled
    sm_config_set_out_shift(&c, true, false, 8);

//configure in section PS2
    // Set the base input pin. pin index 0 is DAT, index 1 is CLK
    sm_config_set_in_pins(&c, base_gpio);

//start the pins as input at start
    uint32_t both_pins = (1u << gpio) | (1u << (gpio + 1) );
    pio_sm_set_pins_with_mask(kbd_pio, kbd_sm, both_pins, 0);
    pio_sm_set_pindirs_with_mask(kbd_pio, kbd_sm, both_pins, 0);
    pio_gpio_init(kbd_pio, gpio);
    pio_gpio_init(kbd_pio, (gpio + 1));
    
    // Shift 8 bits to the right, autopush enabled
    sm_config_set_in_shift(&c, true, true, 8);

    // We don't expect clock faster than 16.7KHz and want no less
    // than 8 SM cycles per keyboard clock.
    float div = (float)clock_get_hz(clk_sys) / (8 * 16700);
    sm_config_set_clkdiv(&c, div);

    //enables IRQ for the statemachine
    pio_set_irq0_source_enabled(kbd_pio, pis_interrupt0 + 0, true); //pis_interrupt0 = PIO_INTR_SM0_LSB 
    irq_set_exclusive_handler(PIO1_IRQ_0, on_pio0_irq0);
    irq_set_enabled(PIO1_IRQ_0, true);


    printf("got here!\n");

    // Ready to go
    pio_sm_init(kbd_pio, kbd_sm, offset, &c);
    pio_sm_set_enabled(kbd_pio, kbd_sm, true);

    while(true)
    {
        sleep_ms(100);
        pio_sm_put_blocking(kbd_pio, kbd_sm,3);
        printf("got here\n");
    }

    


}

