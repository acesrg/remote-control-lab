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

/* include callbacks */
#include <callback_test.h>
#include <callback_classic.h>

// TODO: this should be some kind of pointer
uint8_t URI_TASK = URI_UNDEF;

/*
 * Define the mutexes for both data structures
 * */
SemaphoreHandle_t xMutex_actuator_data;
SemaphoreHandle_t xMutex_sensor_data;

/**
 * This function is called when websocket frame is received.
 *
 * Note: this function is executed on TCP thread and should return as soon
 * as possible.
 */
void websocket_cb(struct tcp_pcb *pcb, uint8_t *data, u16_t data_len, uint8_t mode)
{
    if (URI_TASK == URI_CLASSIC) {
        log_trace("received classic control callback");
        CallbackRvType rv = classic_callback_handler(pcb, data, data_len, mode);
        if (rv == CALLBACK_OK){
            log_trace("classic control callback handled");
        }
        else{
            log_error("classic control callback exited with error status: %d", rv);
        }
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
        xTaskCreate(&classic_controller_task, "classcic_controller_task", 512, (void *) pcb, 2, NULL);
    }
    else if (!strcmp(uri, "/start_pwm")) {
        URI_TASK = URI_START_PWM;
        log_info("Request for propeller start");
        xTaskCreate(&start_pwm_task, "start_pwm_task", 256, (void *) pcb, 2, NULL);
    }
    else if (!strcmp(uri, "/ping")) {
        URI_TASK = URI_PING;
        log_info("Request for ping");
        xTaskCreate(&ping_task, "ping_task", 256, (void *) pcb, 2, NULL);
    }
    else if (!strcmp(uri, "/test")) {
        URI_TASK = URI_PARSE_TEST;
        log_info("Test task");
        xTaskCreate(&test_task, "test_task", 256, (void *) pcb, 2, NULL);
    }
    log_trace("task %s created", uri);
}

void httpd_task(void *pvParameters)
{
    /* initialize mutexes for database interaction */
    xMutex_actuator_data = xSemaphoreCreateMutex();
    xMutex_sensor_data = xSemaphoreCreateMutex();

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
