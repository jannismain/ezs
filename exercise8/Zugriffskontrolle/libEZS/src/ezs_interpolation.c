#include "ezs_gcd.h"
#include <stddef.h>
#include <assert.h>

float linear_interpolation(float first, float second, float alpha) {
    float ret = (1.0f - alpha) * first + alpha * second;
    printf("%f * %f + %f * %f = %f\n", 1.0f - alpha, first, alpha, second, ret);
    return ret;
}

void ezs_resample_linear(float in[], size_t in_length, float out[],
                         size_t out_length) {
    size_t lcm = ezs_lowest_common_multiple(in_length, out_length);
    size_t size = lcm + 1;
    float tmp[size];


    {
        size_t in_stride = size / (in_length - 1);
        size_t i = 0;

        for (i = 0; i < in_length - 1; ++i) {
            size_t j = 0;

            for (j = 0; j < in_stride; ++j) {
                float alpha = ((float) (j)) / ((float) in_stride - 1);

                size_t first_position = i;
                size_t second_position = i + 1;

                size_t target_position = i * in_stride + j;
                assert(target_position < size);

                tmp[target_position] = linear_interpolation(in[first_position],
                                       in[second_position], alpha);
            }
        }

        tmp[size - 1] = in[in_length - 1];
    }

    {
        size_t out_stride = lcm / (out_length - 1);
        size_t i = 0;

        for (i = 0; i < out_length; ++i) {
            size_t source_position = i * out_stride;
            assert(source_position < size);

            out[i] = tmp[source_position];
        }

        out[out_length - 1] = tmp[size - 1];
    }
}

/*
int main()
{
#define IN_LENGTH 512
#define OUT_LENGTH 320
	float input[IN_LENGTH];
	{
		int i;
		for (i = 0; i < IN_LENGTH; ++i) {
			input[i] = i + 1;
		}
	}
	float output[OUT_LENGTH];

	ezs_resample_linear(input, IN_LENGTH, output, OUT_LENGTH);
	int i;
	for (i = 0; i < IN_LENGTH; ++i) {
		printf("%f ", input[i]);
	}
	printf("\n");
	for (i = 0; i < OUT_LENGTH; ++i) {
		printf("%f ", output[i]);
	}
	printf("\n");
}
*/
