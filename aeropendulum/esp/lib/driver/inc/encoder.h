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
 /** \file encoder.h */
#ifndef EXAMPLES_PROJECT_LIB_DRIVER_INC_ENCODER_H_
#define EXAMPLES_PROJECT_LIB_DRIVER_INC_ENCODER_H_

/**
 * \brief   Initialize the encoder and setup interrupts.
 *
 *  Given pin_a and pin_b of the encoder, initialize
 *  the quadrature encoder. Set up the interrupts
 *  for the given pins and populate the structure.
 */
void quadrature_encoder_init(uint8_t pin_a, uint8_t pin_b);

/**
 * \brief   Getter for the value of encoder global.
 */
uint16_t get_encoder_value();

/**
 * \brief   Setter for the value of encoder global.
 */
void set_encoder_value(uint16_t value);

#endif /* EXAMPLES_PROJECT_LIB_DRIVER_INC_ENCODER_H_ */
