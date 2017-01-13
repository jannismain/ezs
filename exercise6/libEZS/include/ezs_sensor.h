#ifndef EZS_SENSOR_H_INCLUDED
#define EZS_SENSOR_H_INCLUDED

#include <inttypes.h>

//alias to map slide prototypes to implementation
#define ezs_adc_init ezs_sensors_init

/*!
 * \brief This function has to be called exactly once so get_sensor_(a|b|c)_value()
 *        and ezs_adc_get yield meaningfull results.
 */
void ezs_sensors_init(void);

/**
 * \brief Returns the current value of sensor A.
 * @return the value of the sensor.
 */
uint16_t get_sensor_a_value(void);

/**
 * \brief Returns the current value of sensor B.
 * @return the value of the sensor.
 */
uint16_t get_sensor_b_value(void);

/**
 * \brief Returns the current value of sensor C.
 * @return the value of the sensor.
 */
uint16_t get_sensor_c_value(void);

/**
 * \brief Returns the current value of the ezs ADC.
 * @return the value of the sensor.
 */
uint8_t ezs_adc_get(void);

#endif
