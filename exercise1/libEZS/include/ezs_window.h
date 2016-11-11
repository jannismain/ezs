#ifndef EZS_DSP_WINDOW_H
#define EZS_DSP_WINDOW_H

/**
 * \file ezs_window.h
 * \ingroup dsp
 * \brief Various FFT window implementations
 * \author Florian Franzmann
 * \note only for internal use.
 */

/**
 * \brief This function calculates the weights for a rectangulage window
 * \return always returns 1.0
 */
float ezs_rect_window(float alpha, int position, int length);

/**
 * \brief This function calculates the weights for a blackman window
 */
float ezs_blackman_window(float alpha, int position, int length);

/*!
 * \brief This function calculates the weights for a hamming window
 */
float ezs_hamming_window(float alpha, int position, int length);

#endif // EZS_DSP_WINDOW_H
