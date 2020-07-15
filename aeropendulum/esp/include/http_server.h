#include <log.h>


#define LED_PIN 2


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
