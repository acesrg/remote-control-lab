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

#include <log.h>

#include <cgi_utils.h>
#include <ssi_utils.h>

extern uint8_t SYSTEM_LOG_LEVEL;


const char *logger_level_cgi_handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[]) {
    uriParamsType logger_cgi_params[2] = {{"verb", "0", false},
                                          {"value", "0", false}};

    retval_t rv;

    rv = decode_uri_params(
        iNumParams,
        (char **) pcParam,
        (char **) pcValue,
        sizeof(logger_cgi_params) / sizeof(logger_cgi_params[0]),
        logger_cgi_params);

    if (rv != RV_OK) {
        return HTTP_CODE(500);
    }

    char param_verb[URI_VARIABLE_VALUE_MAX_LEN];

    rv = uri_param_read(
            "verb",
            param_verb,
            logger_cgi_params,
            sizeof(logger_cgi_params) / sizeof(logger_cgi_params[0]));

    if (rv != RV_OK) {
        return HTTP_CODE(400);
    }

    switch (decode_http_verb(param_verb)) {
        case GET:
            switch (SYSTEM_LOG_LEVEL) {
                case 0:
                    load_ssi_data("LOG_TRACE", sizeof("LOG_TRACE"));
                    break;
                case 1:
                    load_ssi_data("LOG_DEBUG", sizeof("LOG_DEBUG"));
                    break;
                case 2:
                    load_ssi_data("LOG_INFO", sizeof("LOG_INFO"));
                    break;
                case 3:
                    load_ssi_data("LOG_WARN", sizeof("LOG_WARN"));
                    break;
                case 4:
                    load_ssi_data("LOG_ERROR", sizeof("LOG_ERROR"));
                    break;
                default:
                    return HTTP_CODE(500);
            }
            return "/response.ssi";

        case POST: {
            char param_value[URI_VARIABLE_VALUE_MAX_LEN];

            rv = uri_param_read(
                    "value",
                    param_value,
                    logger_cgi_params,
                    sizeof(logger_cgi_params) / sizeof(logger_cgi_params[0]));

            if (rv != RV_OK) {
                return HTTP_CODE(400);
            }

            if (are_strings_equal(param_value, "LOG_TRACE")) {
                SYSTEM_LOG_LEVEL = LOG_TRACE;
            } else if (are_strings_equal(param_value, "LOG_DEBUG")) {
                SYSTEM_LOG_LEVEL = LOG_DEBUG;
            } else if (are_strings_equal(param_value, "LOG_INFO")) {
                SYSTEM_LOG_LEVEL = LOG_INFO;
            } else if (are_strings_equal(param_value, "LOG_WARN")) {
                SYSTEM_LOG_LEVEL = LOG_WARN;
            } else if (are_strings_equal(param_value, "LOG_ERROR")) {
                SYSTEM_LOG_LEVEL = LOG_ERROR;
            } else {
                log_error("Wrong log level selected, keeping %d", SYSTEM_LOG_LEVEL);
                return HTTP_CODE(400);
            }

            log_info("Changing log level to: %s", param_value);
            log_set_level(SYSTEM_LOG_LEVEL);
            return HTTP_CODE(202);

        default:
            return HTTP_CODE(400);
        }
    }
}
