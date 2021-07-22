/*
 * Copyright 2021 ACES.
 *third party local libsin the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */ 
/** \file main.c */
/* standard */
#include <string.h>

/* third party libs */
#include <FreeRTOS.h>
#include <task.h>
#include <espressif/esp_common.h>
#include <espressif/user_interface.h>
#include <esp/uart.h>

/* third party local libs */
#include <log.h>

/* configuration includes */
#include <pinout_configuration.h>

/* Variable Global: esta variable es global para
 comunucarse entre threads */
static uint16_t valor_adc;
static uint16_t adc_voltage;

/*Asignación de parametros para ADC*/
#define SYSTEM_VOLTAGE                      3.3     /**< \brief Default system voltage */
#define ADC_RESOLUTION                      1023    /**< \brief ADC resolution excluding 0 */

/**
 * \brief   adc_read.
 */
void adc_read(void *pvParameters) {
    for (;;) {
        valor_adc = sdk_system_adc_read();
        adc_voltage = valor_adc*(SYSTEM_VOLTAGE/ADC_RESOLUTION);
        log_debug("ADC Voltage: %f", adc_voltage);
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}
