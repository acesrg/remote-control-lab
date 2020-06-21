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
    // TODO: write data to fixed pos array
     
    // TODO: read sensor data from fixed pos array

    char composed_json[JSON_SENSOR_MAX_LEN];
    size_t database_size = sizeof(sensor_db)/sizeof(*sensor_db);
    json_simple_compose(composed_json, sensor_db, database_size);
    websocket_write(pcb, (uint8_t *) composed_json, strlen(composed_json), WS_TEXT_MODE);
    
    return CALLBACK_OK;
}
