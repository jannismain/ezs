#include <math.h>
#include <stdlib.h>

#include "ezs_sensor.h"
#include "ezs_counter.h"

static const unsigned int SENSOR_FREQ[]   = {   1,   4,   8 };
static const float SENSOR_NOISE_PERCENT[] = { 0.1, 0.2, 0.1 };

#define SENSOR_COUNT (sizeof SENSOR_FREQ / sizeof SENSOR_FREQ[0])

static float SENSOR_OMEGA[SENSOR_COUNT];
static float SENSOR_NOISE[SENSOR_COUNT];
static const float AMPLITUDE = UINT16_MAX / 4;

void ezs_sensors_init(void) {
	ezs_counter_init();

	unsigned int i;
	for (i = 0; i < SENSOR_COUNT; ++i) {
		SENSOR_OMEGA[i] = 2. * M_PI * SENSOR_FREQ[i];
		SENSOR_NOISE[i] = AMPLITUDE * SENSOR_NOISE_PERCENT[i];
	}
}

static uint16_t get_sensor_value(unsigned int sensor_id, cyg_uint32 ticks) {

	cyg_resolution_t resolution = ezs_counter_get_resolution();

	float time = (ticks * (float) resolution.dividend) / (resolution.divisor * 1e9f);

	float res = sinf(SENSOR_OMEGA[sensor_id] * time);
	res *= AMPLITUDE; // Amplify.

	int16_t noise = 0;
	if (SENSOR_NOISE[sensor_id] != 0.0) {
		noise = rand()
			% ((int) (SENSOR_NOISE[sensor_id] * 2)) // Cap to the sensor noise factor
			- SENSOR_NOISE[sensor_id];              // Shift into the signed value
	}

	uint16_t value =
		res                // The result as signed value
		+ (UINT16_MAX / 2) // Scale up to unsigned value.
		+ noise;           // Add the noise, skrillex style.

	return value;
}

uint16_t get_sensor_a_value(void) {
	cyg_uint32 ticks = ezs_counter_get();
	return get_sensor_value(0, ticks);
}

uint16_t get_sensor_b_value(void) {
	cyg_uint32 ticks = ezs_counter_get();
	return get_sensor_value(1, ticks);
}

uint16_t get_sensor_c_value(void) {
	cyg_uint32 ticks = ezs_counter_get();
	return get_sensor_value(2, ticks);
}

uint8_t ezs_adc_get() {
	cyg_uint32 ticks = ezs_counter_get();

	uint64_t val_a = get_sensor_value(0, ticks);
	uint64_t val_b = get_sensor_value(1, ticks);

  //scale to 8bit
  uint8_t ret = ((val_a+val_b)*UINT8_MAX )/(2*UINT16_MAX);
  return ret;
}
