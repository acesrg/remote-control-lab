#include <espressif/esp_common.h>
#include <esp8266.h>
#include <httpd/httpd.h>
#include <json_parser.h>
#include <callback_classic.h>
#include <http_server.h>
#include <classic_controller.h>


extern SimpleJSONType actuator_db[1];
extern SimpleJSONType sensor_db[2];


CallbackRvType classic_callback_handler(struct tcp_pcb *pcb, uint8_t *data, u16_t data_len, uint8_t mode){
    
    /*
     * first thing to do when entering the callback for the classic
     * controller is to write the sensor data to the websocket. This
     * way we avoid adding the parsing time to the websocket latency.
     * */
    char composed_json[JSON_SENSOR_MAX_LEN];
    size_t database_size = sizeof(sensor_db)/sizeof(*sensor_db);
    ParseRvType compose_rv = json_simple_compose(composed_json, sensor_db, database_size);
    if (compose_rv != PARSE_OK){
        return CALLBACK_COMPOSE_ERROR;
    }
    websocket_write(pcb, (uint8_t *) composed_json, strlen(composed_json), WS_TEXT_MODE);
    
    /*
     * then, once the response was written to the websocket start
     * processing the input data.
     *
     * since the classic controller database is being used, and the
     * json shold have the form of {"duty": %f}. We pass directly
     * the structure (initialized with said form).
     *
     * after this call, actuator_db.value should be updated and 
     * ready to use.
     * */
    ParseRvType parse_rv = quick_get_value((const char *) data, actuator_db);
    if (parse_rv != PARSE_OK) {
        return CALLBACK_PARSE_ERROR;
    }

    /* if we get to this poin everything went ok! */
    return CALLBACK_OK;
}
