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
#ifndef EXAMPLES_PROJECT_LIB_CGI_INC_CGI_UTILS_H_
#define EXAMPLES_PROJECT_LIB_CGI_INC_CGI_UTILS_H_

#include <stdbool.h>
#include <stddef.h>

#define HTTP_CODE(CODE) ("/" #CODE ".txt")


#define URI_VARIABLE_NAME_MAX_LEN   16
#define URI_VARIABLE_VALUE_MAX_LEN  16


typedef enum http_verb_t {
    GET,
    POST,
    PUT,
    DELETE,
    UNSUPPORTED,
} http_verb_t;

typedef struct uriParamsType {
    const char name[URI_VARIABLE_NAME_MAX_LEN];
    char value[URI_VARIABLE_VALUE_MAX_LEN];
    bool updated;
} uriParamsType;


typedef enum cgiUtilsRvType {
    CGI_OK,
    VARIABLE_NOT_FOUND,
    VALUE_NOT_UPDATED,
} cgiUtilsRvType;

/**
 * \brief   Compares two strings.
 * \param   str1 pointer to a const char
 * \param   str2 pointer to a const char
 * \retval 	bool true if strings are equal, otherwise false
 */
bool are_strings_equal(const char *str1, const char *str2);

/**
 * \brief   Decodes an string into an HTTP verb.
 * \param   verb Pointer to a const char
 * \retval 	http_verb_t An http-verb, like GET, POST, PUT, DELETE or UNSUPPORTED
 */
http_verb_t decode_http_verb(const char *verb);

/**
 * \brief   Retrieves parameters from an URI.
 * \param[in]   num_params An integer specifying the quantity of params in the URI
 * \param[in]   params A pointer to an array of strings, containing the uri params
 * \param[in]   params A pointer to an array of strings, containing the uri values
 * \param[in]   num_available_params An integer, the quantity of params expected
 * \param[out]  available_params, An special struct with the parameters and the
 *              corresponding placeholders for the param values to be written
 * \retval
 */
cgiUtilsRvType decode_uri_params(
    int num_params,
    char *params[],
    char *values[],
    size_t num_available_params,
    uriParamsType *available_params
);

cgiUtilsRvType uri_param_read(
    const char *param,
    char *val,
    uriParamsType *params,
    size_t num_params
);
#endif /* EXAMPLES_PROJECT_LIB_CGI_INC_CGI_UTILS_H_ */
