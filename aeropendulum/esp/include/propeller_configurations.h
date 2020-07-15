/*
 *
 * Propeller defaults
 *
 * */
#define DEFAULT_PROPELLER_PWM_FREQUENCY_HZ  100
#define DEFAULT_PROPELLER_PWM_POWERON_DUTY  0x1C2A
#define DEFAULT_PROPELLER_PWM_PIN           14

typedef struct PwmConfigType {
    uint16_t frequency_hz;
    uint16_t poweron_duty;
    uint16_t pin;
} PwmConfigType;
