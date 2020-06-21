#include "jsmn.h"
#include "log.h"

#define MAX_ACTUATOR_TOKENS 8
#define TOKEN_NAME_MAX_LEN  16
#define TOKEN_FLOAT_MAX_LEN 16
#define JSON_ACTUATOR_MAX_LEN   64
#define JSON_SENSOR_MAX_LEN   64

typedef enum DetokenizeRvType {
    DETOKENIZE_OK,
    DETOKENIZE_ERROR
} DetokenizeRvType;

typedef enum ParseRvType {
    PARSE_OK,
    PARSE_ERROR
} ParseRvType;

typedef enum TokenCompareRvType {
    TOKEN_MATCHES,
    TOKEN_DIFFERS
} TokenCompareRvType;

typedef struct SimpleJSONType {
    const char name[TOKEN_NAME_MAX_LEN];
    float value;
} SimpleJSONType;


/**
 * \brief   generate json from array of SimpleJSONType.
 * \note    this function generates a string in json
 *          format with the arguments of structures of
 *          type SimpleJSONType. That is names and values
 *          Is important to note that this implementation
 *          may be sluggish, and could be optimized with
 *          some effort.
 * \argument    &result: char* output json string, this
 *              string should be big enough to hold the
 *              data, otherwise could cause memory
 *              allocation errors
 * \argument    &inputs: SimpleJSONType* array of
 *              structures
 *              len: size_t the number of SimpleJSONType
 *              structures to be gathered.
 * \return  ParseRvType: status code
 */
ParseRvType json_simple_compose(char *result, SimpleJSONType *inputs, size_t len);


/**
 * \brief   gathers a floating point value from json_string
 *
 * \argument    &json_string: the string from which the
 *              value will be taken.
 * \argument    &stored_data: a SimpleJSONType structure
 *              array, each structure contains:
 *                  name: the name of the variable that
 *                  will be searched.
 *                  value: the resulting float will brief
 *                  stored in this address.
 *  \return     ParseRvType: status code
 */
ParseRvType quick_get_value(const char *json_string, SimpleJSONType *json_data);
