#include <espressif/esp_common.h>
#include <esp8266.h>
#include <esp/uart.h>
#include <string.h>
#include <stdio.h>
#include <FreeRTOS.h>
#include <task.h>
#include <ssid_config.h>
#include <httpd/httpd.h>
#include <http_server.h>
#include <json_parser.h>
#include <testing.h>

TestRvType test_uri_parsing(struct tcp_pcb *pcb, uint8_t *data, u16_t data_len, uint8_t mode){

        printf("Parse test \n inputs: %s \n",data);
        SimpleJSONType received_data[1];
        strcpy(received_data[0].name, "duty");
        quick_get_value((const char *) data, received_data);
        
        
        char composed_echo_json[JSON_ACTUATOR_MAX_LEN];
        size_t received_data_size = sizeof(received_data)/sizeof(*received_data); 
        json_simple_compose(composed_echo_json, received_data, received_data_size);     
        
        websocket_write(pcb, (uint8_t *) composed_echo_json, strlen(composed_echo_json), WS_TEXT_MODE);
        
        printf("\n");
        for(int i = 0; i < received_data_size; i++){
            printf("Parsing result: (name: %s, value:%f) \n", received_data[i].name, received_data[i].value);
        }
        printf("Composition result: %s \n",composed_echo_json);
        return TEST_OK;
}
