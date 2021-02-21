/*
 * Copyright 2020 Marco Miretti.
 *
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */
#include <espressif/esp_common.h>
#include <espressif/user_interface.h>
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
#include <test_cgi.h>
#include <telemetry_cgi.h>
#include <logger_cgi.h>
#include <telemetry_callback.h>
#include <ssi_utils.h>

// TODO(marcotti): this should be some kind of pointer
uint8_t URI_TASK = URI_UNDEF;
uint8_t SYSTEM_LOG_LEVEL = LOG_WARN;

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
void websocket_cb(struct tcp_pcb *pcb, uint8_t *data, u16_t data_len, uint8_t mode) {
    if (URI_TASK == URI_WS_STREAM) {
        log_trace("received ws stream callback, someone is talking");
        CallbackRvType rv = telemetry_callback_handler(pcb, data, data_len, mode);
        if (rv == CALLBACK_OK)
            log_trace("ws stream callback handled");
        else
            log_error("ws stream callback exited with error status: %d", rv);
    } else if (URI_TASK == URI_PARSE_TEST) {
        log_info("received test callback");
        TestRvType rv = test_uri_parsing(pcb, data, data_len, mode);
        if (rv == TEST_OK)
            log_info("test: everything of from this end");
        else
            log_error("test: something went wrong");
    } else {
        log_error("callback method unrecognized %d", URI_TASK);
    }
}

/**
 * This function is called when new websocket is open
 */
void websocket_open_cb(struct tcp_pcb *pcb, const char *uri) {
    if (!strcmp(uri, "/stream")) {
        URI_TASK = URI_WS_STREAM;
        log_info("Request for websocket stream");
        xTaskCreate(&send_telemetry_task, "send_telemetry", 512, (void *) pcb, 2, NULL);
        xTaskCreate(&update_actuators_task, "update_actuators", 512, (void *) 1, 2, NULL);
    } else if (!strcmp(uri, "/start_pwm")) {
        URI_TASK = URI_START_PWM;
        log_info("Request for propeller start");
        xTaskCreate(&start_pwm_task, "start_pwm_task", 256, (void *) pcb, 2, NULL);
    } else if (!strcmp(uri, "/ping")) {
        URI_TASK = URI_PING;
        log_info("Request for ping");
        xTaskCreate(&ping_task, "ping_task", 256, (void *) pcb, 2, NULL);
    } else if (!strcmp(uri, "/test")) {
        URI_TASK = URI_PARSE_TEST;
        log_info("Test task");
        xTaskCreate(&test_task, "test_task", 256, (void *) pcb, 2, NULL);
    }
    log_trace("task %s created", uri);
}

void httpd_task(void *pvParameters) {
    /* initialize mutexes for database interaction */
    xMutex_actuator_data = xSemaphoreCreateMutex();
    xMutex_sensor_data = xSemaphoreCreateMutex();

    tCGI pCGIs[] = {
        {"/test", (tCGIHandler) test_cgi_handler},
        {"/test/resource", (tCGIHandler) test_resource_cgi_handler},
        {"/logger/level", (tCGIHandler) logger_level_cgi_handler},
        {"/telemetry/period", (tCGIHandler) telemetry_period_cgi_handler},
    };


    /*
     * only use one SSI, the handler will just replace
     * the tag with the string from a pointer+len. The
     * function set_ssi_response() should be called
     * before invoking the SSI enabled file. */
    const char *pcConfigSSITags[] = {
        "response",
    };

    /* register handlers and start the server */
    http_set_cgi_handlers(pCGIs, sizeof (pCGIs) / sizeof (pCGIs[0]));
    http_set_ssi_handler((tSSIHandler) ssi_handler, pcConfigSSITags,
            sizeof (pcConfigSSITags) / sizeof (pcConfigSSITags[0]));

    /* register handlers and start the server */
    websocket_register_callbacks((tWsOpenHandler) websocket_open_cb, (tWsHandler) websocket_cb);
    httpd_init();

    for (;;) {}
}

void user_init(void) {
    uart_set_baud(0, 115200);
    log_set_level(SYSTEM_LOG_LEVEL);
    log_info("SDK version:%s ", sdk_system_get_sdk_version());

    struct sdk_station_config config = {
        .ssid = WIFI_SSID,
        .password = WIFI_PASS,
    };

    struct ip_info station_ip;

    /* TODO: this should be more automatic */
    IP4_ADDR(&station_ip.ip, 192, 168, 100, 41);
    IP4_ADDR(&station_ip.gw, 192, 168, 100, 1);
    IP4_ADDR(&station_ip.netmask, 255, 255, 255, 0);
    sdk_wifi_station_dhcpc_stop();
    vTaskDelay(50);

    /* required to call wifi_set_opmode before station_set_config */
    sdk_wifi_set_opmode(STATION_MODE);
    log_trace("static ip set status : %d", sdk_wifi_set_ip_info(STATION_IF, &station_ip));
    sdk_wifi_station_set_config(&config);
    sdk_wifi_station_connect();

    /* turn off LED */
    gpio_enable(LED_PIN, GPIO_OUTPUT);
    gpio_write(LED_PIN, true);

    /* initialize tasks */
    xTaskCreate(&httpd_task, "HTTP Daemon", 256, NULL, 2, NULL);
}
