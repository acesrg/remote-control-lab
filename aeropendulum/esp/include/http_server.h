#include <log.h>

#define LED_PIN 2

#define ENCODER_PIN_A 12
#define ENCODER_PIN_B 13

enum URI_TASK
{
    URI_UNDEF,
	URI_CLASSIC,
    URI_START_PWM,
    URI_PING,
    URI_PARSE_TEST,
};

void start_pwm_task(void *pvParameter);

void classic_controller_task(void *pvParameter);

void ping_task(void *pvParameter);

void test_task(void *pvParameter);
