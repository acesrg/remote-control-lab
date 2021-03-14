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
#include <string.h>

#include <cgi_utils.h>
#include <log.h>


bool are_strings_equal(const char *str1, const char *str2) {
    if (strcmp(str1, str2) == 0) {
        return true;
    } else {
        return false;
    }
}

http_verb_t decode_http_verb(const char *verb) {
    if (are_strings_equal(verb, "GET")) {
        return GET;
    } else if (are_strings_equal(verb, "POST")) {
        return POST;
    } else if (are_strings_equal(verb, "PUT")) {
        return PUT;
    } else if (are_strings_equal(verb, "DELETE")) {
        return DELETE;
    } else {
        return UNSUPPORTED;
    }
}

retval_t decode_uri_params(
    int num_params,
    char *params[],
    char *values[],
    size_t num_available_params,
    uriParamsType *available_params
) {
    for (size_t i = 0; i < num_params; i++) {
        log_trace("Trying to decode param: %s", params[i]);
        for (size_t j = 0; j < num_available_params; j++) {
            log_trace("Coparing with param: %s", available_params[j].name);
            if (strcmp((const char *) params[i], available_params[j].name) == 0) {
                log_trace("Matched, will copy the value");
                strcpy(available_params[j].value, values[i]);
                log_trace("Value copied, set updated -> true");
                available_params[j].updated = true;
                log_trace("Decoded param: %s with value: %s", params[i], available_params[j].value);
            }
        }
    }
    return RV_OK;
}

retval_t uri_param_read(
    const char *param,
    char *val,
    uriParamsType *params,
    size_t num_params
) {
    for (size_t i = 0; i < num_params; i++) {
        if (strcmp(params[i].name, param) == 0) {
            log_trace("uri_param_read: Found %s", param);
            if (params[i].updated) {
                strcpy(val, params[i].value);
                return RV_OK;
            } else {
                return RV_EXTERNAL_ERROR;
            }
        }
    }
    return RV_ERROR;
}
