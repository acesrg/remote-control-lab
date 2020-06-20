#include <espressif/esp_common.h>
#include <esp8266.h>
#include <string.h>
#include <stdio.h>

typedef enum CallbackRvType {
    CALLBACK_OK,
    CALLBACK_ERROR
} CallbackRvType;

CallbackRvType classic_callback_handler(struct tcp_pcb *pcb, uint8_t *data, u16_t data_len, uint8_t mode);
