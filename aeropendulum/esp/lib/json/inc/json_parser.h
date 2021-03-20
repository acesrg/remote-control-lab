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
/** \file json_parser.h */
#ifndef EXAMPLES_PROJECT_LIB_JSON_INC_JSON_PARSER_H_
#define EXAMPLES_PROJECT_LIB_JSON_INC_JSON_PARSER_H_

#include <retval.h>

#define MAX_ACTUATOR_TOKENS     8   /**< \brief Max tokens (num) that an actuator json can handle*/
#define TOKEN_NAME_MAX_LEN      16  /**< \brief Max len (chars) of JSON token name */
#define TOKEN_FLOAT_MAX_LEN     16  /**< \brief Max len (chars) of JSON float */
#define JSON_ACTUATOR_MAX_LEN   64  /**< \brief Max len (chars) of JSON actuator string */
#define JSON_SENSOR_MAX_LEN     64  /**< \brief Max len (chars) of JSON sensor string */


/**
 * \typedef simple_json_t
 * \brief   A struct tought to be converted to a JSON string.
 */
typedef struct simple_json_t {
    const char name[TOKEN_NAME_MAX_LEN];    /**< \brief name of a parameter */
    uint16_t value;                         /**< \brief value of the parameter */
} simple_json_t;


/**
 * \brief   generate json from array of simple_json_t.
 * \note    this function generates a string in json
 *          format with the arguments of structures of
 *          type simple_json_t. That is names and values
 *          Is important to note that this implementation
 *          may be sluggish, and could be optimized with
 *          some effort.
 * \param    &result: char* output json string, this
 *              string should be big enough to hold the
 *              data, otherwise could cause memory
 *              allocation errors
 * \param    &inputs: simple_json_t* array of
 *              structures
 * \param    len: size_t the number of simple_json_t
 *              structures to be gathered.
 * \return  retval_t: status code
 */
retval_t json_simple_compose(char *result, simple_json_t *inputs, size_t len);


/**
 * \brief   gathers a floating point value from json_string
 *
 * \param    &json_string: the string from which the
 *              value will be taken.
 * \param    &json_data: a simple_json_t structure
 *              array, each structure contains:
 *                  name: the name of the variable that
 *                  will be searched.
 *                  value: the resulting float will brief
 *                  stored in this address.
 *  \return     retval_t: status code
 */
retval_t quick_get_value(const char *json_string, simple_json_t *json_data);

#endif /* EXAMPLES_PROJECT_LIB_JSON_INC_JSON_PARSER_H_ */
