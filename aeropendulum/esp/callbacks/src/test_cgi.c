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

#include <log.h>
#include <httpd/httpd.h>

#include <cgi_utils.h>
#include <ssi_utils.h>
#include <json_parser.h>

extern uint8_t SYSTEM_LOG_LEVEL;
static char dummy_test_resource[LWIP_HTTPD_MAX_TAG_INSERT_LEN];
static uint16_t global_resource_child_a;
static uint16_t global_resource_child_b;

const char *test_cgi_handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[]) {
    uriParamsType test_cgi_params[2] = {{"verb", "0", false},
                                        {"say", "0", false}};

    retval_t rv;

    rv = decode_uri_params(
               iNumParams,
               (char **) pcParam,
               (char **) pcValue,
               sizeof(test_cgi_params) / sizeof(test_cgi_params[0]),
               test_cgi_params);
    if (rv != RV_OK) goto labeled_return;

    char param_verb[URI_VARIABLE_VALUE_MAX_LEN];

    rv = uri_param_read(
               "verb",
               param_verb,
               test_cgi_params,
               sizeof(test_cgi_params) / sizeof(test_cgi_params[0]));
    if (rv != RV_OK) goto labeled_return; /*< this param is mandatory*/


    switch (decode_http_verb(param_verb)) {
        case POST: {
                char say_what[URI_VARIABLE_VALUE_MAX_LEN];
                rv = uri_param_read(
                           "say",  //<-- no voy a borrar esto, mejor voy a probar mandar un json y ver como lo recive
                           say_what,
                           test_cgi_params,
                           sizeof(test_cgi_params) / sizeof(test_cgi_params[0]));
                if (rv != RV_OK) goto labeled_return;

                if (SYSTEM_LOG_LEVEL > LOG_INFO) {
                    log_set_level(LOG_INFO);
                    log_info("Since this is a test, i lowered the logger to info level, hi UART!");
                }

                log_info("You wanted me to say: %s", say_what);

                if (SYSTEM_LOG_LEVEL > LOG_INFO) {
                    log_info("Restore log level, bye UART!");
                    log_set_level(SYSTEM_LOG_LEVEL);
                }
            }
            break;

        /* verb not supported */
        default:
            return HTTP_CODE(400);
    }


    /*
     * labeled exit for the callback, assings an HTTP code
     * according to the intermediate functions retvals
     * */
    labeled_return:
    switch (rv) {
        case RV_OK:
            return HTTP_CODE(200);

        case RV_ERROR:
            return HTTP_CODE(500);

        case RV_EXTERNAL_ERROR:
            return HTTP_CODE(400);

        default:
            return HTTP_CODE(500);
    }
}

const char *test_resource_cgi_handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[]) {
    uriParamsType test_cgi_params[2] = {{"verb", "0", false},
                                        {"value", "0", false}};

    retval_t rv;

    rv = decode_uri_params(
               iNumParams,
               (char **) pcParam,
               (char **) pcValue,
               sizeof(test_cgi_params) / sizeof(test_cgi_params[0]),
               test_cgi_params);
    if (rv != RV_OK) goto labeled_return;

    char param_verb[URI_VARIABLE_VALUE_MAX_LEN];

    rv = uri_param_read(
               "verb",
               param_verb,
               test_cgi_params,
               sizeof(test_cgi_params) / sizeof(test_cgi_params[0]));
    if (rv != RV_OK) goto labeled_return; /*< this param is mandatory*/


    switch (decode_http_verb(param_verb)) {
        case GET: {
                load_ssi_data(dummy_test_resource, sizeof(dummy_test_resource));
                return "/response.ssi";
            }

        case POST: {
                char param_value[URI_VARIABLE_VALUE_MAX_LEN];
                rv = uri_param_read(
                           "value",
                           param_value,
                           test_cgi_params,
                           sizeof(test_cgi_params) / sizeof(test_cgi_params[0]));
                if (rv != RV_OK) goto labeled_return; /*< mandatory for post*/
                snprintf(dummy_test_resource, sizeof(param_value), param_value);
                return HTTP_CODE(202);
            }
            break;

        /* verb not supported */
        default:
            return HTTP_CODE(400);
    }


    /*
     * labeled exit for the callback, assings an HTTP code
     * according to the intermediate functions retvals
     * */
    labeled_return:
    switch (rv) {
        case RV_OK:
            return HTTP_CODE(200);

        case RV_ERROR:
            return HTTP_CODE(500);

        case RV_EXTERNAL_ERROR:
            return HTTP_CODE(400);

        default:
            return HTTP_CODE(500);
    }
}

const char *test_parent_resource_cgi_handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[]) {
    uriParamsType test_cgi_params[2] = {{"verb", "0", false},
                                        {"value", "0", false}};

    retval_t rv;

    rv = decode_uri_params(
               iNumParams,
               (char **) pcParam,
               (char **) pcValue,
               sizeof(test_cgi_params) / sizeof(test_cgi_params[0]),
               test_cgi_params);
    if (rv != RV_OK) goto labeled_return;

    char param_verb[URI_VARIABLE_VALUE_MAX_LEN];

    rv = uri_param_read(
               "verb",
               param_verb,
               test_cgi_params,
               sizeof(test_cgi_params) / sizeof(test_cgi_params[0]));
    if (rv != RV_OK) goto labeled_return; /*< this param is mandatory*/


    switch (decode_http_verb(param_verb)) {
        case GET: {
                static char composed_parent_resource[LWIP_HTTPD_MAX_TAG_INSERT_LEN];

                simple_json_t parent_resource[2] = {{"child_a", global_resource_child_a},
                                                    {"child_b", global_resource_child_b}};

                size_t resource_size = sizeof(parent_resource)/sizeof(*parent_resource);
                retval_t parse_rv = json_simple_compose(composed_parent_resource, parent_resource, resource_size);

                if (parse_rv != RV_OK) goto labeled_return;

                load_ssi_data(composed_parent_resource, sizeof(composed_parent_resource));
                return "/response.ssi";
            }

        case POST: {
                return HTTP_CODE(501);
            }
            break;

        /* verb not supported */
        default:
            return HTTP_CODE(400);
    }


    /*
     * labeled exit for the callback, assings an HTTP code
     * according to the intermediate functions retvals
     * */
    labeled_return:
    switch (rv) {
        case RV_OK:
            return HTTP_CODE(200);

        case RV_ERROR:
            return HTTP_CODE(500);

        case RV_EXTERNAL_ERROR:
            return HTTP_CODE(400);

        default:
            return HTTP_CODE(500);
    }
}

const char *test_child_a_resource_cgi_handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[]) {
    uriParamsType test_cgi_params[2] = {{"verb", "0", false},
                                        {"value", "0", false}};

    retval_t rv;

    rv = decode_uri_params(
               iNumParams,
               (char **) pcParam,
               (char **) pcValue,
               sizeof(test_cgi_params) / sizeof(test_cgi_params[0]),
               test_cgi_params);
    if (rv != RV_OK) goto labeled_return;

    char param_verb[URI_VARIABLE_VALUE_MAX_LEN];

    rv = uri_param_read(
               "verb",
               param_verb,
               test_cgi_params,
               sizeof(test_cgi_params) / sizeof(test_cgi_params[0]));
    if (rv != RV_OK) goto labeled_return; /*< this param is mandatory*/


    switch (decode_http_verb(param_verb)) {
        case GET: {
                char response_str[LWIP_HTTPD_MAX_TAG_INSERT_LEN];
                snprintf(response_str, LWIP_HTTPD_MAX_TAG_INSERT_LEN, "0x%04X", global_resource_child_a);
                load_ssi_data(response_str, sizeof(response_str));
                return "/response.ssi";
            }

        case POST: {
                char param_value[URI_VARIABLE_VALUE_MAX_LEN];
                rv = uri_param_read(
                           "value",
                           param_value,
                           test_cgi_params,
                           sizeof(test_cgi_params) / sizeof(test_cgi_params[0]));
                if (rv != RV_OK) goto labeled_return; /*< mandatory for post*/
                global_resource_child_a = strtoul(param_value, NULL, 16);
                return HTTP_CODE(202);
            }
            break;

        /* verb not supported */
        default:
            return HTTP_CODE(400);
    }


    /*
     * labeled exit for the callback, assings an HTTP code
     * according to the intermediate functions retvals
     * */
    labeled_return:
    switch (rv) {
        case RV_OK:
            return HTTP_CODE(200);

        case RV_ERROR:
            return HTTP_CODE(500);

        case RV_EXTERNAL_ERROR:
            return HTTP_CODE(400);

        default:
            return HTTP_CODE(500);
    }
}

const char *test_child_b_resource_cgi_handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[]) {
    uriParamsType test_cgi_params[2] = {{"verb", "0", false},
                                        {"value", "0", false}};

    retval_t rv;

    rv = decode_uri_params(
               iNumParams,
               (char **) pcParam,
               (char **) pcValue,
               sizeof(test_cgi_params) / sizeof(test_cgi_params[0]),
               test_cgi_params);
    if (rv != RV_OK) goto labeled_return;

    char param_verb[URI_VARIABLE_VALUE_MAX_LEN];

    rv = uri_param_read(
               "verb",
               param_verb,
               test_cgi_params,
               sizeof(test_cgi_params) / sizeof(test_cgi_params[0]));
    if (rv != RV_OK) goto labeled_return; /*< this param is mandatory*/


    switch (decode_http_verb(param_verb)) {
        case GET: {
                char response_str[LWIP_HTTPD_MAX_TAG_INSERT_LEN];
                snprintf(response_str, LWIP_HTTPD_MAX_TAG_INSERT_LEN, "0x%04X", global_resource_child_b);
                load_ssi_data(response_str, sizeof(response_str));
                return "/response.ssi";
            }

        case POST: {
                char param_value[URI_VARIABLE_VALUE_MAX_LEN];
                rv = uri_param_read(
                           "value",
                           param_value,
                           test_cgi_params,
                           sizeof(test_cgi_params) / sizeof(test_cgi_params[0]));
                if (rv != RV_OK) goto labeled_return; /*< mandatory for post*/
                global_resource_child_b = strtoul(param_value, NULL, 16);
                return HTTP_CODE(202);
            }
            break;

        /* verb not supported */
        default:
            return HTTP_CODE(400);
    }


    /*
     * labeled exit for the callback, assings an HTTP code
     * according to the intermediate functions retvals
     * */
    labeled_return:
    switch (rv) {
        case RV_OK:
            return HTTP_CODE(200);

        case RV_ERROR:
            return HTTP_CODE(500);

        case RV_EXTERNAL_ERROR:
            return HTTP_CODE(400);

        default:
            return HTTP_CODE(500);
    }
}
