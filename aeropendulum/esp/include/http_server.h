
#define LED_PIN 2
#define SYSTEM_REFRESH_RATE_ms 20
#define CLASSIC_SENSOR_DATA_LEN 64 

enum URI_TASK
{
    URI_UNDEF,
	URI_CLASSIC,
    URI_PING,
};


void classic_controller_task(void *pvParameter);

void ping_task(void *pvParameter);
