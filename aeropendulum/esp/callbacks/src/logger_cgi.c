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

#include <logger_cgi.h>
#include <cgi_utils.h>
#include <log.h>

extern uint8_t SYSTEM_LOG_LEVEL;


const char *logger_cgi_handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[]) {
    uriParamsType logger_cgi_params[2] = {{"verb", "0", false},
                                          {"level", "0", false}};

    cgiUtilsRvType rv;

    rv = decode_uri_params(
        iNumParams,
        (char **) pcParam,
        (char **) pcValue,
        sizeof(logger_cgi_params) / sizeof(logger_cgi_params[0]),
        logger_cgi_params);

    if (rv != CGI_OK) {
        return HTTP_CODE(500);
    }

    char param_verb[URI_VARIABLE_VALUE_MAX_LEN];

    rv = uri_param_read(
            "verb",
            param_verb,
            logger_cgi_params,
            sizeof(logger_cgi_params) / sizeof(logger_cgi_params[0]));

    if (rv != CGI_OK) {
        return HTTP_CODE(400);
    }

    char param_level[URI_VARIABLE_VALUE_MAX_LEN];

    rv = uri_param_read(
            "level",
            param_level,
            logger_cgi_params,
            sizeof(logger_cgi_params) / sizeof(logger_cgi_params[0]));

    if (rv != CGI_OK) {
        return HTTP_CODE(400);
    }

    switch (decode_http_verb(param_verb)) {
        case GET:
            return HTTP_CODE(501);
            break;

        case POST:
            if (are_strings_equal(param_level, "LOG_TRACE")) {
                SYSTEM_LOG_LEVEL = LOG_TRACE;
            } else if (are_strings_equal(param_level, "LOG_DEBUG")) {
                SYSTEM_LOG_LEVEL = LOG_DEBUG;
            } else if (are_strings_equal(param_level, "LOG_INFO")) {
                SYSTEM_LOG_LEVEL = LOG_INFO;
            } else if (are_strings_equal(param_level, "LOG_WARN")) {
                SYSTEM_LOG_LEVEL = LOG_WARN;
            } else if (are_strings_equal(param_level, "LOG_ERROR")) {
                SYSTEM_LOG_LEVEL = LOG_ERROR;
            } else {
                log_error("Wrong log level selected, keeping %d", SYSTEM_LOG_LEVEL);
                return HTTP_CODE(400);
            }

            log_info("Changing log level to: %s", param_level);
            log_set_level(SYSTEM_LOG_LEVEL);
            return HTTP_CODE(202);

        default:
            return HTTP_CODE(400);
    }
}
