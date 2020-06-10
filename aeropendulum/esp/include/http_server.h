
#define LED_PIN 2

/*
 * Classic mode definitions
 * */
#define CLASSIC_SENSOR_DB_LEN 64 
#define CLASSIC_ACTUATOR_DB_LEN 64 
#define CLASSIC_SYSTEM_REFRESH_RATE_ms 20

enum URI_TASK
{
    URI_UNDEF,
	URI_CLASSIC,
    URI_PING,
};


void classic_controller_task(void *pvParameter);

void ping_task(void *pvParameter);
