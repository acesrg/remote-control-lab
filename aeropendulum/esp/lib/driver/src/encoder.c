/*
 * Copyright 2020 Marco Miretti.
 *
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */
#include <esp8266.h>

#include <log.h>
#include <json_parser.h>

#include <encoder.h>

typedef struct EncoderObjectType {
    uint8_t pin_a;
    uint8_t pin_b;
    size_t  last_state;
    uint16_t value;
} EncoderObjectType;

typedef enum EncoderEventType {
    NO_MEASURE,
    PIN_A_RISE,
    PIN_B_RISE,
    PIN_A_FALL,
    PIN_B_FALL
} EncoderEventType;

#define INCREMENT (uint8_t) (PIN_B_RISE - PIN_A_RISE)
#define INCREMENT_ (uint8_t) (PIN_A_RISE - PIN_B_FALL)

#define DECREMENT (uint8_t) (PIN_A_RISE - PIN_B_RISE)
#define DECREMENT_ (uint8_t) (PIN_B_FALL - PIN_A_RISE)

/* globals to be used by isr */
EncoderObjectType encoder = {0, 0, NO_MEASURE, 0};

void encoder_intr_handler(uint8_t gpio_num);

uint16_t get_encoder_value() {
    return encoder.value;
}

void set_encoder_value(uint16_t value) {
    encoder.value = value;
}

/**
 * \brief   Initialize the encoder and setup interrupts.
 * \note    Given pin_a and pin_b of the encoder, initialize
 *          the quadrature encoder. Set up the interrupts
 *          for the given pins and populate the structure.
 **/
void quadrature_encoder_init(uint8_t pin_a, uint8_t pin_b) {
    log_trace("Init quadrature encoder");
    log_trace("Set gpio as input");
    gpio_enable(pin_a, GPIO_INPUT);
    gpio_enable(pin_b, GPIO_INPUT);

    encoder.pin_a = pin_a;
    encoder.pin_b = pin_b;

    encoder.value = 0;

    gpio_set_interrupt(pin_a, GPIO_INTTYPE_EDGE_ANY, encoder_intr_handler);
    gpio_set_interrupt(pin_b, GPIO_INTTYPE_EDGE_ANY, encoder_intr_handler);
}


/**
 * \brief   Detects the nature of the encoder interrupt
 * \note    Given the pin number and the state of said
 *          pin, it can define if the interruption was
 *          caused by a rising or falling edge on either
 *          pin_a or pin_b on the encoder.
 *
 * \param   gpio_num: the pin number
 * \retval  EncoderEventType (enum): the type of said
 *          interrupt (rising, falling and which pin)
 **/
EncoderEventType detect_event_nature(uint8_t gpio_num) {
    if (gpio_num == encoder.pin_a) {
        if (gpio_read(gpio_num))
            return PIN_A_RISE;
        else
            return PIN_A_FALL;
    } else if (gpio_num == encoder.pin_b) {
        if (gpio_read(gpio_num))
            return PIN_B_RISE;
        else
            return PIN_B_FALL;
    } else {
        return NO_MEASURE;
    }
}

/**
 * \brief quadrature encoder interrupt service routine
 * \note    According to the event nature decides if the
 *      counter should be incremented, decremented or
 *      kept as is.
 *
 * \param gpio_num: the pin number
 **/
void encoder_intr_handler(uint8_t gpio_num) {
    EncoderEventType current_event_nature = detect_event_nature(gpio_num);

    if (encoder.last_state != NO_MEASURE) {
        uint8_t diff = current_event_nature - encoder.last_state;
        if (diff == INCREMENT || diff == INCREMENT_)
            encoder.value++;
        else if (diff == DECREMENT || diff == DECREMENT_)
            encoder.value--;
    }
    encoder.last_state = current_event_nature;
}
