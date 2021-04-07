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
/** \file http_server.c */
/* standard */
#include <string.h>

/* third party libs */
#include <FreeRTOS.h>
#include <semphr.h>
#include <task.h>
#include <espressif/esp_common.h>
#include <espressif/user_interface.h>
#include <httpd/httpd.h>
#include <esp/uart.h>

/* third party local libs */
#include <log.h>

/* local libs */
#include <ssi_utils.h>
#include <retval.h>

/* project tasks */
#include <send_telemetry.h>
#include <update_actuators.h>

/* project callbacks/cgi */
#include <logger_cgi.h>
#include <propeller_cgi.h>
#include <telemetry_callback.h>
#include <telemetry_cgi.h>
#include <test_cgi.h>

/* configuration includes */
#include <pinout_configuration.h>
#include <private_ssid_config.h>

uint8_t SYSTEM_LOG_LEVEL = LOG_WARN;

/** \brief Mutex to avoid reading the actuator value while is being written */
SemaphoreHandle_t xMutex_actuator_data;

/**
 * \brief   Called when websocket frame is received.
 *
 * \warning Is executed on TCP thread and should return as soon
 *          as possible.
 */
void websocket_cb(struct tcp_pcb *pcb, uint8_t *data, u16_t data_len, uint8_t mode) {
    log_trace("received ws stream callback, someone is talking");
    retval_t rv = telemetry_callback_handler(pcb, data, data_len, mode);
    if (rv == RV_OK)
        log_trace("ws stream callback handled");
    else
        log_error("ws stream callback exited with error status: %d", rv);
}

/**
 * \brief   Called when new websocket is opened.
 *
 *  When ws://$HOST_IP/stream is hit, it will create the tasks to receive and send telemetry.
 */
void websocket_open_cb(struct tcp_pcb *pcb, const char *uri) {
    if (!strcmp(uri, "/stream")) {
        log_info("Request for websocket stream");
        BaseType_t rv = xTaskCreate(&send_telemetry_task, "send_telemetry", 512, (void *) pcb, 2, NULL);
        if (rv == pdPASS) {
            log_trace("task 'send_telemetry' created");
        } else {
            log_error("could not allocate memory for 'send_telemetry' task");
        }

        rv = xTaskCreate(&update_actuators_task, "update_actuators", 512, (void *) pcb, 2, NULL);
        if (rv == pdPASS) {
            log_trace("task 'update_actuators' created");
        } else {
            log_error("could not allocate memory for 'update_actuators' task");
        }
    }
}

/**
 * \brief   HTTP server task.
 *
 *  Sets mutexes, maps HTTP uris, sets handlers and callbacks (SSI, CGI and websockets).
 *
 * \param   *pvParameters: The TCP socket.
 */
void httpd_task(void *pvParameters) {
    /* initialize mutexes for database interaction */
    xMutex_actuator_data = xSemaphoreCreateMutex();

    tCGI pCGIs[] = {
        {"/test", (tCGIHandler) test_cgi_handler},
        {"/test/resource", (tCGIHandler) test_resource_cgi_handler},
        {"/test/parent_resource", (tCGIHandler) test_parent_resource_cgi_handler},
        {"/test/parent_resource/child_a", (tCGIHandler) test_child_a_resource_cgi_handler},
        {"/test/parent_resource/child_b", (tCGIHandler) test_child_b_resource_cgi_handler},
        {"/logger/level", (tCGIHandler) logger_level_cgi_handler},
        {"/telemetry/period", (tCGIHandler) telemetry_period_cgi_handler},
        {"/propeller/pwm/status", (tCGIHandler) propeller_pwm_status_cgi_handler},
    };


    /**
     * \note On SSI tags.
     *      Only use one SSI, the handler will just replace
     *      the tag with the string from a pointer+len. The
     *      function set_ssi_response() should be called
     *      before invoking the SSI enabled file. */
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

/**
 * \brief Program's entrypoint.
 */
void user_init(void) {
    uart_set_baud(0, 115200);
    log_set_level(SYSTEM_LOG_LEVEL);
    log_info("SDK version:%s ", sdk_system_get_sdk_version());

    struct sdk_station_config config = {
        .ssid = WIFI_SSID,
        .password = WIFI_PASS,
    };

    struct ip_info station_ip;

    /**
     * \todo The IP address set should be more automatic.
     */
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
    gpio_enable(ONBOARD_LED_PIN, GPIO_OUTPUT);
    gpio_write(ONBOARD_LED_PIN, true);

    /* initialize tasks */
    xTaskCreate(&httpd_task, "HTTP Daemon", 256, NULL, 2, NULL);
}
