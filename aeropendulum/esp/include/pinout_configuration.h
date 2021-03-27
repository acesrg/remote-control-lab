/*
 * Copyright 2021 Marco Miretti.
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
/** \file pinout_configuration.h */
#ifndef EXAMPLES_PROJECT_INCLUDE_PINOUT_CONFIGURATION_H_
#define EXAMPLES_PROJECT_INCLUDE_PINOUT_CONFIGURATION_H_

/**
 * Pinout for the ESP8266 D1 MINI
 * https://user-images.githubusercontent.com/26353057/87876217-f3333a00-c9ac-11ea-9a11-893db497dc48.png
 */
/*******************************
 *      PIN DESCRIPTION     PIN
 *******************************/
#define ONBOARD_LED_PIN     2   /**< \brief Led on top of the D1 MINI board */

#define ENCODER_PIN_A       12  /**< \brief Pin connected to the encoder signal A */
#define ENCODER_PIN_B       13  /**< \brief Pin connected to the encoder signal B */

#endif /* EXAMPLES_PROJECT_INCLUDE_PINOUT_CONFIGURATION_H_ */
