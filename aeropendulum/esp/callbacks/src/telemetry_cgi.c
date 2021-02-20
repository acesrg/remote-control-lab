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
#include <stdlib.h>

#include <httpd/httpd.h>
#include <log.h>

#include <cgi_utils.h>
#include <ssi_utils.h>

#include <telemetry_cgi.h>
#include <send_telemetry.h>

extern uint16_t TELEMETRY_RATE_ms;

const char *telemetry_period_cgi_handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[]) {
    uriParamsType telemetry_cgi_params[2] = {{"verb", "0", false},
                                             {"value", "0", false}};

    cgiUtilsRvType rv;

    rv = decode_uri_params(
        iNumParams,
        (char **) pcParam,
        (char **) pcValue,
        sizeof(telemetry_cgi_params) / sizeof(telemetry_cgi_params[0]),
        telemetry_cgi_params);

    if (rv != CGI_OK) {
        return HTTP_CODE(500);
    }

    char param_verb[URI_VARIABLE_VALUE_MAX_LEN];

    rv = uri_param_read(
            "verb",
            param_verb,
            telemetry_cgi_params,
            sizeof(telemetry_cgi_params) / sizeof(telemetry_cgi_params[0]));

    if (rv != CGI_OK) {
        return HTTP_CODE(400);
    }

    switch (decode_http_verb(param_verb)) {
        case GET: {
            int response_len;
            char response[LWIP_HTTPD_MAX_TAG_INSERT_LEN];
            response_len = sprintf(response, "%d Hz", TELEMETRY_RATE_ms);
            load_ssi_data(response, response_len);
            return "/response.ssi";
        }

        case POST: {
            char param_value[URI_VARIABLE_VALUE_MAX_LEN];

            rv = uri_param_read(
                    "value",
                    param_value,
                    telemetry_cgi_params,
                    sizeof(telemetry_cgi_params) / sizeof(telemetry_cgi_params[0]));

            if (rv != CGI_OK) {
                return HTTP_CODE(400);
            }

            int value_int = atoi(param_value);

            if ((value_int > MAX_TELEMETRY_RATE_ms) || (value_int < MIN_TELEMETRY_RATE_ms)) {
                log_error("Period of %d [ms] is out of bounds.\n"
                          "(max: %d ms, min: %d ms), keeping %d ms.", TELEMETRY_RATE_ms);
                return HTTP_CODE(403);
            } else {
                log_info("Changing telemetry frequency to: %d", value_int);
                TELEMETRY_RATE_ms = value_int;
            }
            return HTTP_CODE(202);

        default:
            return HTTP_CODE(400);
        }
    }
}
