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
/** \file cgi_utils.h */
#ifndef EXAMPLES_PROJECT_LIB_CGI_INC_CGI_UTILS_H_
#define EXAMPLES_PROJECT_LIB_CGI_INC_CGI_UTILS_H_

#include <stdbool.h>
#include <stddef.h>

#include <retval.h>

/**
 * \brief Helper macro to translate from error code number
 *        to a pre-crafted response file.
 */
#define HTTP_CODE(CODE) ("/" #CODE ".txt")

#define URI_VARIABLE_NAME_MAX_LEN   16  /**< Max length of a query variable's name */
#define URI_VARIABLE_VALUE_MAX_LEN  16  /**< Max length of a query variable's value */


/**
 * \brief HTTP protocol request verbs enumerator.
 */
typedef enum http_verb_t {
    GET,
    POST,
    PUT,
    DELETE,
    UNSUPPORTED,
} http_verb_t;


/**
 * \brief Storage for URI parameters.
 */
typedef struct uriParamsType {
    const char name[URI_VARIABLE_NAME_MAX_LEN]; /**< Name of the query variable */
    char value[URI_VARIABLE_VALUE_MAX_LEN];     /**< Value of the query variable */
    bool updated;                               /**< Update status of the query variable */
} uriParamsType;

/**
 * \brief Compares two strings for equality.
 *
 * \param *str1: Pointer to the first char of first string.
 * \param *str2: Pointer to the first char of second string.
 * \return bool: true if equal, false otherwise.
 */
bool are_strings_equal(const char *str1, const char *str2);

/**
 * \brief Gets a value in the verb enumerator from a string.
 * \param *verb: Pointer to the first char of the verb string.
 * \return http_verb_t: The equivalent verb in the enumerator.
 */
http_verb_t decode_http_verb(const char *verb);

/**
 * \brief Given a HTTP query, extract expected query variables.
 * \param[in] num_params: Quantity of parameters in the query.
 * \param[in] *params[]: Pointer to first char of first query variable name.
 * \param[in] *values[]: Pointer to first char of first query variable value.
 * \param[in] num_available_params: Quantity of variables we expect to extract.
 * \param[in,out] *available_params: Pointer to the first uriParamsType, containing
 *                in it's "name" field, the name to the first expected parameter.
 * \return retval_t status code.
 */
retval_t decode_uri_params(
    int num_params,
    char *params[],
    char *values[],
    size_t num_available_params,
    uriParamsType *available_params
);


/**
 * \brief Given a uriParamsType array, read the value of a specific variable.
 * \param[in] *param: Pointer to first char of the variable's name, which value
 *        we expect to read.
 * \param[out] *val: Pointer to first char of the output.
 * \param[in] *params: Pointer to first structure of the variables array.
 * \param[in] num_params: Quantity of variables in the "params" array.
 * \return retval_t: status code.
 */
retval_t uri_param_read(
    const char *param,
    char *val,
    uriParamsType *params,
    size_t num_params
);
#endif /* EXAMPLES_PROJECT_LIB_CGI_INC_CGI_UTILS_H_ */
