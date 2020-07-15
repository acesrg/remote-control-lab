#include <espressif/esp_common.h>
#include <esp8266.h>
#include <esp/uart.h>
#include <string.h>
#include <stdio.h>
#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include <ssid_config.h>
#include <httpd/httpd.h>
#include <http_server.h>
#include <classic_controller.h>
#include <json_parser.h>
#include <pwm.h>
#include <propeller_configurations.h>

// TODO: ugh such an ugly global
extern uint8_t URI_TASK;

PwmConfigType pwm_config = {
                            DEFAULT_PROPELLER_PWM_FREQUENCY_HZ,
                            DEFAULT_PROPELLER_PWM_POWERON_DUTY,
                            DEFAULT_PROPELLER_PWM_PIN
                            };

void start_pwm_task(void *pvParameter){
    
    uint8_t pins[1];
    log_trace("task started");
    log_trace("pwm_init(1, [%d])", pwm_config.pin);
    pins[0] = pwm_config.pin;
    pwm_init(1, pins, false);

    log_trace("pwm_set_freq %d", pwm_config.frequency_hz);
    pwm_set_freq(pwm_config.frequency_hz);

    log_trace("pwm set duty");
    pwm_set_duty(pwm_config.poweron_duty);

    log_trace("pwm_start");
    pwm_start();
    
    URI_TASK = URI_UNDEF;    
    log_trace("disconected, delete task");
    vTaskDelete(NULL);
}
