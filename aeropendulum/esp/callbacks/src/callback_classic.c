#include <espressif/esp_common.h>
#include <esp8266.h>
#include <httpd/httpd.h>
#include <json_parser.h>
#include <callback_classic.h>
#include <http_server.h>

CallbackRvType classic_callback_handler(struct tcp_pcb *pcb, uint8_t *data, u16_t data_len, uint8_t mode){
    // TODO: write data to fixed pos array
    
    // TODO: read sensor data from fixed pos array

    float angle = 0;
    uint8_t error = 0;
    char sensor_data[CLASSIC_SENSOR_DB_LEN];
    int len = snprintf(sensor_data, sizeof(sensor_data),
                       "{\"angle\" : %f,"
                       " \"error\" : %u}", angle, error);
    
    websocket_write(pcb, (uint8_t *) sensor_data, len, WS_TEXT_MODE);
    
    return CALLBACK_OK;
}
