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
#ifndef EXAMPLES_PROJECT_LIB_JSON_INC_JSON_PARSER_H_
#define EXAMPLES_PROJECT_LIB_JSON_INC_JSON_PARSER_H_

#include <jsmn.h>
#include <log.h>

#define MAX_ACTUATOR_TOKENS 8
#define TOKEN_NAME_MAX_LEN  16
#define TOKEN_FLOAT_MAX_LEN 16
#define JSON_ACTUATOR_MAX_LEN   64
#define JSON_SENSOR_MAX_LEN   64

typedef enum DetokenizeRvType {
    DETOKENIZE_OK,
    DETOKENIZE_ERROR
} DetokenizeRvType;

typedef enum ParseRvType {
    PARSE_OK,
    PARSE_ERROR
} ParseRvType;

typedef enum TokenCompareRvType {
    TOKEN_MATCHES,
    TOKEN_DIFFERS
} TokenCompareRvType;

typedef struct SimpleJSONType {
    const char name[TOKEN_NAME_MAX_LEN];
    uint16_t value;
} SimpleJSONType;


/**
 * \brief   generate json from array of SimpleJSONType.
 * \note    this function generates a string in json
 *          format with the arguments of structures of
 *          type SimpleJSONType. That is names and values
 *          Is important to note that this implementation
 *          may be sluggish, and could be optimized with
 *          some effort.
 * \argument    &result: char* output json string, this
 *              string should be big enough to hold the
 *              data, otherwise could cause memory
 *              allocation errors
 * \argument    &inputs: SimpleJSONType* array of
 *              structures
 *              len: size_t the number of SimpleJSONType
 *              structures to be gathered.
 * \return  ParseRvType: status code
 */
ParseRvType json_simple_compose(char *result, SimpleJSONType *inputs, size_t len);


/**
 * \brief   gathers a floating point value from json_string
 *
 * \argument    &json_string: the string from which the
 *              value will be taken.
 * \argument    &stored_data: a SimpleJSONType structure
 *              array, each structure contains:
 *                  name: the name of the variable that
 *                  will be searched.
 *                  value: the resulting float will brief
 *                  stored in this address.
 *  \return     ParseRvType: status code
 */
ParseRvType quick_get_value(const char *json_string, SimpleJSONType *json_data);

#endif /* EXAMPLES_PROJECT_LIB_JSON_INC_JSON_PARSER_H_ */
