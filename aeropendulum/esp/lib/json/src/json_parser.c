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
#include <string.h>
#include <stdlib.h>

#include <jsmn.h>
#include <log.h>

#include <json_parser.h>

typedef enum token_compare_t {
    TOKEN_MATCHES,
    TOKEN_DIFFERS
} token_compare_t;

/**
 * \brief       Takes json argument (float) given json string and jsmn token
 * \argument    &result: is the result float passed by pointer
 * \argument    &input:  the input json string
 * \argument    token:  a jsmn token that points to that json argument
 * \return      retval_t: status code
 */
retval_t detokenize_float(float *result, const char *input, jsmntok_t *token) {
    char result_str[TOKEN_FLOAT_MAX_LEN] = {0};

    if (token->type != JSMN_PRIMITIVE) {
        log_error("detokenize_string: expected primitive type token");
        return RV_ERROR;
    }

    memcpy(result_str , &input[token->start], token->end - token->start);
    *result = strtof(result_str, NULL);
    return RV_OK;
}


/**
 * \brief       Takes json argument (hex) given json string and jsmn token
 * \argument    &result: is the result hex passed by pointer
 * \argument    &input:  the input json string
 * \argument    token:  a jsmn token that points to that json argument
 * \return      retval_t: status code
 */
retval_t detokenize_hex(uint16_t *result, const char *input, jsmntok_t *token) {
    char result_str[TOKEN_FLOAT_MAX_LEN] = {0};

    if (token->type != JSMN_STRING) {
        log_error("detokenize_string: expected string type token");
        return RV_ERROR;
    }

    memcpy(result_str , &input[token->start], token->end - token->start);
    *result = (uint16_t) strtoul(result_str, NULL, 16);
    return RV_OK;
}

/**
 * \brief   quick token compare using strncmp
 * \argument    &input: the input json string
 * \argument    &token: the token that places the string
 * \argument    &expected: expected string
 * \return      token_compare_t, comparisson value, that
 *              is:
 *                  TOKEN_MATCHES or
 *                  TOKEN_DIFFERS
 */
token_compare_t compare_token(const char *input, jsmntok_t *token, const char *expected) {
    if (token->type == JSMN_STRING && (int)strlen(expected) == token->end - token->start &&
            strncmp(input + token->start, expected, token->end - token->start) == 0) {
        return TOKEN_MATCHES;
    }
    return TOKEN_DIFFERS;
}


retval_t quick_get_value(const char *json_string, simple_json_t *stored_data) {
    jsmn_parser p;
    jsmntok_t t[MAX_ACTUATOR_TOKENS];
    int r;

    jsmn_init(&p);
    r = jsmn_parse(&p, json_string, strlen(json_string), t, sizeof(t)/sizeof(t[0]));
    if (r < 0) {
        log_error("jsmn_parse: failed to parse JSON: %d", r);
        return RV_ERROR;
    }
    /* Assume the top-level element is an object */
    if (r < 1 || t[0].type != JSMN_OBJECT) {
        log_error("quick_get_value: object expected");
    }

    for (int i = 0; i < r; i++) {
        /* extract string from first token */
        if (compare_token(json_string, &t[i], stored_data[0].name) == TOKEN_MATCHES) {
            /* extract duty value from secon token */
            detokenize_hex(&stored_data[0].value, json_string, &t[i+1]);
        }
    }
    return RV_OK;
}


retval_t json_simple_compose(char *result, simple_json_t *inputs, size_t len) {
    result[0] = '\0';
    snprintf(result, JSON_SENSOR_MAX_LEN, "{");
    for (int i = 0; i < len; i++) {
        char stringed_pair[TOKEN_NAME_MAX_LEN+TOKEN_FLOAT_MAX_LEN+8];
        snprintf(stringed_pair, JSON_SENSOR_MAX_LEN, "\"%s\": \"0x%04X\"", inputs[i].name, inputs[i].value);
        strcat(result, stringed_pair);
        if (i != len-1) {
            strcat(result, ", ");
        }
    }
    strcat(result, "}");
    return RV_OK;
}
