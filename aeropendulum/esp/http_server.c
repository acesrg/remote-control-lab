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
#include <testing.h>

// TODO: this should be some kind of pointer
uint8_t URI_TASK = URI_UNDEF;

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
        char sensor_data[CLASSIC_SENSOR_DB_LEN];
        int len = snprintf(sensor_data, sizeof(sensor_data),
                           "{\"angle\" : %f,"
                           " \"error\" : %u}", angle, error);
        
        websocket_write(pcb, (uint8_t *) sensor_data, len, WS_TEXT_MODE);
    }
    
    else if (URI_TASK == URI_PARSE_TEST) {
        log_info("received test callback");
        TestRvType rv = test_uri_parsing(pcb, data, data_len, mode);
        if (rv == TEST_OK){
            log_info("test: everything of from this end");
        }
        else{
            log_error("test: something went wrong");
        }
    }
    else {
        log_error("callback method unrecognized");
    }
}

/**
 * This function is called when new websocket is open
 */
void websocket_open_cb(struct tcp_pcb *pcb, const char *uri)
{
    if (!strcmp(uri, "/classic")) {
        URI_TASK = URI_CLASSIC;
        log_info("Request for classic control");
        xTaskCreate(&classic_controller_task, "classcic_controller_task", 256, (void *) pcb, 2, NULL);
    }
    else if (!strcmp(uri, "/ping")) {
        URI_TASK = URI_PING;
        log_info("Request for ping");
        xTaskCreate(&ping_task, "ping_task", 256, (void *) pcb, 2, NULL);
    }
    else if (!strcmp(uri, "/test")) {
        URI_TASK = URI_PARSE_TEST;
        log_info("Test task");
        xTaskCreate(&test_task, "test_task", 512, (void *) pcb, 2, NULL);
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
    log_info("SDK version:%s ", sdk_system_get_sdk_version());

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
