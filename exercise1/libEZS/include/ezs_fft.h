#ifndef EZS_DSP_FFT_H
#define EZS_DSP_FFT_H


/**
 * \defgroup dsp Digital Signal Processing (DSP)
 */

/**
 * \file ezs_fft.h Fast fourier transformation
 * \ingroup dsp
 * \brief calculates the power density spectrum of in[]
 */



/**
 * \brief Calculates the PDS of a given time domain signal
 * \param in input, time domain signal.
 * \param out output, out[0] to out[length / 2 - 1] contains the power density spectrum of in[] in dB
 * \param length of in[]. Must be a power of 2. The size of out[] must be at least length / 2 - 1.
 *
 * \note Be careful, only rudimentary error checking is performed.
 * \see http://en.wikipedia.org/wiki/Spectral_density
 */
void ezs_power_density_spectrum(float in[], float out[], int length);

/**
 * \brief Calculates the PDS of a given time domain signal
 * \param in input, time domain signal.
 * \param out output, contains the power density spectrum of in[] in dB.
 * \param length of in[]. Must be a power of 2. The size of out[] must be at least length/2 - 1.
 *
 * This is a convenience function that calls ezs_power_density_spectrum().
 *
 * \note Be careful, only rudimentary error checking is performed.
 * \see http://en.wikipedia.org/wiki/Spectral_density
 */
void ezs_easy_pds(cyg_uint32 in[], float out[], int length);


#endif // EZS_DSP_FFT_H
