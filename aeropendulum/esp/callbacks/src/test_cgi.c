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
#include <test_cgi.h>
#include <cgi_utils.h>
#include <log.h>

extern uint8_t SYSTEM_LOG_LEVEL;


const char *test_cgi_handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[]) {
    uriParamsType test_cgi_params[2] = {{"say", "0", false},
                                        {"verb", "0", false}};

    if (SYSTEM_LOG_LEVEL > LOG_INFO) {
        log_set_level(LOG_INFO);
        log_info("Since this is a test, i lowered the logger to info level, hi UART!");
    }

    cgiUtilsRvType rv;

    rv = decode_uri_params(
        iNumParams,
        (char **) pcParam,
        (char **) pcValue,
        sizeof(test_cgi_params) / sizeof(test_cgi_params[0]),
        test_cgi_params);

    char say_what[URI_VARIABLE_VALUE_MAX_LEN];

    rv = uri_param_read(
            "say",
            say_what,
            test_cgi_params,
            sizeof(test_cgi_params) / sizeof(test_cgi_params[0]));

    if (rv == CGI_OK) {
            log_info("You wanted me to say: %s", say_what);
    }

    if (SYSTEM_LOG_LEVEL > LOG_INFO) {
        log_info("Restore log level, bye UART!");
        log_set_level(SYSTEM_LOG_LEVEL);
    }

    switch (rv) {
        case CGI_OK:
            return HTTP_CODE(200);

        case VARIABLE_NOT_FOUND:
            return HTTP_CODE(500);

        case VALUE_NOT_UPDATED:
            return HTTP_CODE(400);

        default:
            return HTTP_CODE(500);
    }
}
