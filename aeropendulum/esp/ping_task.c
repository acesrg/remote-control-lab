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

// TODO: ugh 
extern uint8_t URI_TASK;

void ping_task(void *pvParameter)
{
    struct tcp_pcb *pcb = (struct tcp_pcb *) pvParameter;
    uint8_t response[] = "pong";
    websocket_write(pcb, response, sizeof(response) - 1, WS_TEXT_MODE);

    // when task stops mark as undefined
    URI_TASK = URI_UNDEF;    
    printf("Disconected, delete task");
    vTaskDelete(NULL);
}
