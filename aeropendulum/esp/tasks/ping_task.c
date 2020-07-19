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
#include <esp8266.h>
#include <esp/uart.h>
#include <string.h>
#include <stdio.h>
#include <FreeRTOS.h>
#include <task.h>
#include <ssid_config.h>
#include <httpd/httpd.h>
#include <http_server.h>


extern uint8_t URI_TASK;

void ping_task(void *pvParameter) {
    struct tcp_pcb *pcb = (struct tcp_pcb *) pvParameter;
    uint8_t response[] = "pong";
    LOCK_TCPIP_CORE();
    websocket_write(pcb, response, sizeof(response) - 1, WS_TEXT_MODE);
    UNLOCK_TCPIP_CORE();

    // when task stops mark as undefined
    URI_TASK = URI_UNDEF;
    log_trace("disconected, delete task");
    vTaskDelete(NULL);
}
