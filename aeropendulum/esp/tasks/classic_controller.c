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
#include <classic_controller.h>
#include <json_parser.h>

// TODO: ugh such an ugly global
extern uint8_t URI_TASK;


SimpleJSONType actuator_db[1] = {{"duty", 0}};

SimpleJSONType sensor_db[2] = {{"angle", 0},
                               {"error", 0}};

void classic_controller_task(void *pvParameter){
    struct tcp_pcb *pcb = (struct tcp_pcb *) pvParameter;
    uint8_t response[] = "classic control mode";
    LOCK_TCPIP_CORE();
    websocket_write(pcb, response, sizeof(response) -1, WS_TEXT_MODE);
    UNLOCK_TCPIP_CORE();

    while(1){
        // TODO: database read actuator data
        // TODO: actuator set data
        
        
        // TODO: read sensor data
        // TODO: database write sensor data
        vTaskDelay(CLASSIC_SYSTEM_REFRESH_RATE_ms / portTICK_PERIOD_MS);
        
        if(pcb == NULL || pcb->state != ESTABLISHED){
            // when task stops mark as undefined
            URI_TASK = URI_UNDEF;    
            log_trace("disconected, delete task");
            vTaskDelete(NULL);
        }
    }
}
