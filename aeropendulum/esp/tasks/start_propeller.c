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
#include <pwm.h>

// TODO: ugh such an ugly global
extern uint8_t URI_TASK;


void start_propeller_task(void *pvParameter){
    
    uint8_t pins[1];
    log_trace("task started");
    log_trace("pwm_init(1, [14])\n");
    
    pins[0] = 14;
    pwm_init(1, pins, false);

    log_trace("pwm_set_freq(100)");
    pwm_set_freq(100);

    log_trace("pwm set duty");
    pwm_set_duty(7209);

    log_trace("pwm_start");
    pwm_start();
    
    log_trace("wait 1 sec");
    vTaskDelay(1000 / portTICK_PERIOD_MS);

    for (int i = 7600; i > 7209; i -= 10) {
        log_trace("pwm: %d",i);
        pwm_set_duty(i);
        vTaskDelay(500 / portTICK_PERIOD_MS);        
    }
    pwm_set_duty(7209);

    URI_TASK = URI_UNDEF;    
    log_trace("disconected, delete task");
    vTaskDelete(NULL);

}
