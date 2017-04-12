#include "ezs_window.h"
#include <math.h>
#define M_PI	3.14159265358979323846f	/* pi */

#include <assert.h>

float ezs_rect_window(float alpha, int position, int length) {
    return 1.0f;
}

float ezs_blackman_window(float alpha, int position, int length) {
    assert(length > 0);
    assert(position >= 0);
    assert(position < length);
    assert(alpha >= 0.0f);
    assert(1.0f >= alpha);

    float a[3] = {(1.0f - alpha) / 2.0f, 0.5f, alpha / 2.0f};
    return a[0]
           - a[1] * cos((2.0f * M_PI * ((float) position)) / ((float) (length - 1)))
           + a[2] * cos((4.0f * M_PI * ((float) position)) / ((float) (length - 1)));
}

float ezs_hamming_window(float alpha, int position, int length) {
    assert(length > 0);
    assert(position >= 0);
    assert(position < length);

    return 0.54f - 0.46f * cos((2.0f * M_PI * (float) position) / (float) (
                                   length - 1));
}
