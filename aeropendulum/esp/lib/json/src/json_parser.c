#include <espressif/esp_common.h>
#include <esp8266.h>
#include <string.h>
#include <stdio.h>
#include <json_parser.h>


/**
 * \brief       Takes json argument (float) given json string and jsmn token
 * \argument    &result: is the result float passed by pointer
 * \argument    &input:  the input json string
 * \argument    token:  a jsmn token that points to that json argument
 * \return      DetokenizeRvType: status code
 */
DetokenizeRvType detokenize_float(float *result, const char *input, jsmntok_t *token)
{
    char result_str[TOKEN_FLOAT_MAX_LEN] = {0};

    if (token->type != JSMN_PRIMITIVE){
        log_error("detokenize_string: expected primitive type token");
        return DETOKENIZE_ERROR;
    }

    memcpy(result_str , &input[token->start], token->end - token->start);
    *result = strtof(result_str, NULL);
    return DETOKENIZE_OK;
}


/**
 * \brief       Takes json argument (hex) given json string and jsmn token
 * \argument    &result: is the result hex passed by pointer
 * \argument    &input:  the input json string
 * \argument    token:  a jsmn token that points to that json argument
 * \return      DetokenizeRvType: status code
 */
DetokenizeRvType detokenize_hex(uint16_t *result, const char *input, jsmntok_t *token)
{
    char result_str[TOKEN_FLOAT_MAX_LEN] = {0};

    if (token->type != JSMN_STRING){
        log_error("detokenize_string: expected string type token");
        return DETOKENIZE_ERROR;
    }

    memcpy(result_str , &input[token->start], token->end - token->start);
    *result = (uint16_t) strtoul(result_str, NULL, 16);
    return DETOKENIZE_OK;
}

/**
 * \brief   quick token compare using strncmp
 * \argument    &input: the input json string
 * \argument    &token: the token that places the string
 * \argument    &expected: expected string
 * \return      TokenCompareRvType, comparisson value, that
 *              is:
 *                  TOKEN_MATCHES or
 *                  TOKEN_DIFFERS
 */
TokenCompareRvType compare_token(const char *input, jsmntok_t *token, const char *expected)
{
    if (token->type == JSMN_STRING && (int)strlen(expected) == token->end - token->start &&
            strncmp(input + token->start, expected, token->end - token->start) == 0) {
        return TOKEN_MATCHES;
    }
    return TOKEN_DIFFERS;
}


ParseRvType quick_get_value(const char *json_string, SimpleJSONType *stored_data)
{
    jsmn_parser p;
    jsmntok_t t[MAX_ACTUATOR_TOKENS];
    int r;

    jsmn_init(&p);
    r = jsmn_parse(&p, json_string, strlen(json_string), t, sizeof(t)/sizeof(t[0]));
    if (r < 0) {
        log_error("jsmn_parse: failed to parse JSON: %d", r);
        return PARSE_ERROR;
    }
   
    /* Assume the top-level element is an object */
    if (r < 1 || t[0].type != JSMN_OBJECT) {
        log_error("quick_get_value: object expected");
    }

    for(int i = 0; i < r; i++){
        /* extract string from first token */
        if (compare_token(json_string, &t[i], stored_data[0].name) == TOKEN_MATCHES) {
            /* extract duty value from secon token */
            detokenize_hex(&stored_data[0].value, json_string, &t[i+1]);
        }
    }
    return PARSE_OK;
}


ParseRvType json_simple_compose(char *result, SimpleJSONType *inputs, size_t len)
{
    result[0] = '\0';
    strcat(result, "{");
    for(int i = 0; i < len; i++){
        char stringed_pair[TOKEN_NAME_MAX_LEN+TOKEN_FLOAT_MAX_LEN+8];
        sprintf(stringed_pair, "\"%s\": \"0x%04X\"",inputs[i].name, inputs[i].value);
        strcat(result, stringed_pair);
        if(i != len-1){
            strcat(result, ", ");
        }
    }
    strcat(result, "}");
    return PARSE_OK;
}
