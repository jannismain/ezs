#ifndef EZS_INTERPOLATION_H
#define EZS_INTERPOLATION_H

/**
 * \file ezs_interpolation.h
 * \ingroup dsp
 * \brief performs linear resampling of in[] and writes the result to out[]
 * @see http://en.wikipedia.org/wiki/Sample_rate_conversion
 * @see http://en.wikipedia.org/wiki/Linear_interpolation
 */
void ezs_resample_linear(float in[], size_t in_length, float out[],
                         size_t out_length);
#endif // EZS_INTERPOLATION_H
