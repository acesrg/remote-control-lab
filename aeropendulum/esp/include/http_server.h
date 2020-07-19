#include <log.h>


/**
 * Pinout for the ESP8266 D1 MINI
 * https://user-images.githubusercontent.com/26353057/87876217-f3333a00-c9ac-11ea-9a11-893db497dc48.png
 */
#define LED_PIN             2

#define ENCODER_PIN_A       12
#define ENCODER_PIN_B       13

#define PROPELLER_PWM_PIN   14

/**
 * Propeller default configs
 * */
#define DEFAULT_PROPELLER_PWM_FREQUENCY_HZ  100
#define DEFAULT_PROPELLER_PWM_POWERON_DUTY  0x1C2A

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
