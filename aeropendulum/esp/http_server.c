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

// this should be some kind of pointer
uint8_t URI_TASK = URI_UNDEF;

void classic_controller_task(void *pvParameter)
{
    struct tcp_pcb *pcb = (struct tcp_pcb *) pvParameter;
    uint8_t response[] = "classic control mode";
    websocket_write(pcb, response, sizeof(response) -1, WS_TEXT_MODE);
    
    while(1){
        // TODO: database read actuator data
        // TODO: actuator set data
        
        
        // TODO: read sensor data
        // TODO: database write sensor data
        vTaskDelay(SYSTEM_REFRESH_RATE_ms / portTICK_PERIOD_MS);
        
        if(pcb == NULL || pcb->state != ESTABLISHED){
            // when task stops mark as undefined
            URI_TASK = URI_UNDEF;    
            printf("Disconected, delete task");
            vTaskDelete(NULL);
        }
    }

}

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

/**
 * This function is called when websocket frame is received.
 *
 * Note: this function is executed on TCP thread and should return as soon
 * as possible.
 */
void websocket_cb(struct tcp_pcb *pcb, uint8_t *data, u16_t data_len, uint8_t mode)
{
    if (URI_TASK == URI_CLASSIC) {
        // TODO: write data to fixed pos array
        
        // TODO: read sensor data from fixed pos array

        float angle = 0;
        uint8_t error = 0;
        char sensor_data[CLASSIC_SENSOR_DATA_LEN];
        int len = snprintf(sensor_data, sizeof(sensor_data),
                           "{\"angle\" : %f,"
                           " \"error\" : %u}", angle, error);
        
        websocket_write(pcb, (uint8_t *) sensor_data, len, WS_TEXT_MODE);
    }
}

/**
 * This function is called when new websocket is open
 */
void websocket_open_cb(struct tcp_pcb *pcb, const char *uri)
{
    if (!strcmp(uri, "/classic")) {
        URI_TASK = URI_CLASSIC;
        printf("Request for classic control \n");
        xTaskCreate(&classic_controller_task, "classcic_controller_task", 256, (void *) pcb, 2, NULL);
    }
    else if (!strcmp(uri, "/ping")) {
        URI_TASK = URI_PING;
        printf("Request for ping \n");
        xTaskCreate(&ping_task, "ping_task", 256, (void *) pcb, 2, NULL);
    }
}

void httpd_task(void *pvParameters)
{
    /* register handlers and start the server */
    websocket_register_callbacks((tWsOpenHandler) websocket_open_cb,
            (tWsHandler) websocket_cb);
    httpd_init();

    for (;;);
}

void user_init(void)
{
    uart_set_baud(0, 115200);
    printf("SDK version:%s\n", sdk_system_get_sdk_version());

    struct sdk_station_config config = {
        .ssid = WIFI_SSID,
        .password = WIFI_PASS,
    };

    /* required to call wifi_set_opmode before station_set_config */
    sdk_wifi_set_opmode(STATION_MODE);
    sdk_wifi_station_set_config(&config);
    sdk_wifi_station_connect();

    /* turn off LED */
    gpio_enable(LED_PIN, GPIO_OUTPUT);
    gpio_write(LED_PIN, true);

    /* initialize tasks */
    xTaskCreate(&httpd_task, "HTTP Daemon", 256, NULL, 2, NULL);
}
