#include <espressif/esp_common.h>
#include <esp8266.h>
#include <esp/uart.h>
#include <string.h>
#include <stdio.h>
#include <FreeRTOS.h>
#include <task.h>
#include <http_server.h>

#define TEST_WAIT_TIME_ms 500
// TODO: ugh 
extern uint8_t URI_TASK;

void test_task(void *pvParameter)
{
    vTaskDelay(TEST_WAIT_TIME_ms / portTICK_PERIOD_MS);
    URI_TASK = URI_UNDEF;
    log_trace("disconected, delete task");
    vTaskDelete(NULL);
}
