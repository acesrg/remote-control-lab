#include <espressif/esp_common.h>
#include <esp8266.h>
#include <string.h>
#include <stdio.h>

typedef enum TestRvType {
    TEST_OK,
    TEST_ERROR
} TestRvType;

TestRvType test_uri_parsing(struct tcp_pcb *pcb, uint8_t *data, u16_t data_len, uint8_t mode);
