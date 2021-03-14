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
#include <propeller_utils.h>

char propeller_pwm_status[URI_VARIABLE_VALUE_MAX_LEN] = "disabled";

const char *propeller_pwm_status_cgi_handler(
    int iIndex,
    int iNumParams,
    char *pcParam[],
    char *pcValue[]) {

    uriParamsType propeller_pwm_cgi_params[2] = {{"verb", "0", false},
                                          {"value", "0", false}};

    cgiUtilsRvType rv;

    rv = decode_uri_params(
        iNumParams,
        (char **) pcParam,
        (char **) pcValue,
        sizeof(propeller_pwm_cgi_params) / sizeof(propeller_pwm_cgi_params[0]),
        propeller_pwm_cgi_params);

    if (rv != CGI_OK) {
        return HTTP_CODE(500);
    }

    char param_verb[URI_VARIABLE_VALUE_MAX_LEN];

    rv = uri_param_read(
            "verb",
            param_verb,
            propeller_pwm_cgi_params,
            sizeof(propeller_pwm_cgi_params) / sizeof(propeller_pwm_cgi_params[0]));

    if (rv != CGI_OK) {
        return HTTP_CODE(400);
    }

    switch (decode_http_verb(param_verb)) {
        case GET:
            load_ssi_data(propeller_pwm_status, URI_VARIABLE_VALUE_MAX_LEN);
            return "/response.ssi";

        case POST: {
            char param_value[URI_VARIABLE_VALUE_MAX_LEN];

            rv = uri_param_read(
                    "value",
                    param_value,
                    propeller_pwm_cgi_params,
                    sizeof(propeller_pwm_cgi_params) / sizeof(propeller_pwm_cgi_params[0]));

            if (rv != CGI_OK) {
                return HTTP_CODE(400);
            }

            if (are_strings_equal(param_value, "initialized")) {
                log_warn("Initializing propeller...");

                retval_t rv = initialize_propeller();
                if (rv != RV_OK) {
                    log_error("Failed propeller initialization.");
                    return HTTP_CODE(500);
                } else {
                    log_warn("Propeller successfully initialized.");
                    strcpy(propeller_pwm_status, "initialized");
                    return HTTP_CODE(202);
                }
            } else if (are_strings_equal(param_value, "disabled")) {
                log_warn("Disabling propeller...");

                retval_t rv = deinitialize_propeller();
                if (rv != RV_OK) {
                    log_error("Failed propeller deinitialization.");
                    return HTTP_CODE(500);
                } else {
                    log_warn("Propeller successfully disabled.");
                    strcpy(propeller_pwm_status, "disabled");
                    return HTTP_CODE(202);
                }
            } else {
                log_error("Wrong status selected, options are {initialized, disabled}");
                return HTTP_CODE(400);
            }

        default:
            return HTTP_CODE(400);
        }
    }
}
