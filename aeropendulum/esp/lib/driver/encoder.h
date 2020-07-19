#include "espressif/esp_common.h"
#include "esp8266.h"
#include "log.h"

typedef struct EncoderObjectType {
    uint8_t pin_a;
    uint8_t pin_b;
    size_t  last_state;
    uint16_t value;
} EncoderObjectType;

typedef enum EncoderEventType {
    NO_MEASURE,
    PIN_A_RISE,
    PIN_B_RISE,
    PIN_A_FALL,
    PIN_B_FALL
} EncoderEventType;

#define INCREMENT (uint8_t) (PIN_B_RISE - PIN_A_RISE)
#define INCREMENT_ (uint8_t) (PIN_A_RISE - PIN_B_FALL)

#define DECREMENT (uint8_t) (PIN_A_RISE - PIN_B_RISE)
#define DECREMENT_ (uint8_t) (PIN_B_FALL - PIN_A_RISE)

void quadrature_encoder_init(uint8_t pin_a, uint8_t pin_b);

uint16_t get_encoder_value();
void set_encoder_value(uint16_t value);
