#include <espressif/esp_common.h>
#include <esp8266.h>
#include <esp/uart.h>
#include <string.h>
#include <stdio.h>
#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include <ssid_config.h>
#include <httpd/httpd.h>
#include <http_server.h>
#include <classic_controller.h>
#include <json_parser.h>

// TODO: ugh such an ugly global
extern uint8_t URI_TASK;

extern SemaphoreHandle_t xMutex_actuator_data;
extern SemaphoreHandle_t xMutex_sensor_data;

SimpleJSONType actuator_db[1] = {{"duty", 0}};

SimpleJSONType sensor_db[2] = {{"angle", 0},
                               {"error", 0}};

void classic_controller_task(void *pvParameter){
    log_trace("task started");
    struct tcp_pcb *pcb = (struct tcp_pcb *) pvParameter;

    while(1){
        if( xMutex_actuator_data != NULL ){
            /* See if we can obtain the actuator_db mutex */
            if( xSemaphoreTake( xMutex_actuator_data, ( TickType_t ) 100 ) == pdTRUE ){
                uint16_t actuator_duty_value = actuator_db[0].value;
               
                log_trace("set actuator duty: 0x%04X");
                pwm_set_duty(actuator_duty_value);
                xSemaphoreGive( xMutex_actuator_data );
            }
        }

        if( xMutex_sensor_data != NULL ){
            /* See if we can obtain the actuator_db mutex */
            if( xSemaphoreTake( xMutex_sensor_data, ( TickType_t ) 100 ) == pdTRUE ){
                // TODO: read sensor data
                float sensor_angle_value, sensor_error;
                sensor_db[0].value = sensor_angle_value;
                sensor_db[1].value = sensor_error;
                xSemaphoreGive( xMutex_sensor_data );
            }
        }
        
        vTaskDelay(CLASSIC_SYSTEM_REFRESH_RATE_ms / portTICK_PERIOD_MS);
        
        if(pcb == NULL || pcb->state != ESTABLISHED){
            // when task stops mark as undefined
            URI_TASK = URI_UNDEF;    
            log_trace("disconected, delete task");
            vTaskDelete(NULL);
        }
    }
}
