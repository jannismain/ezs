#include "ezs_gcd.h"
#include <math.h>
#include <stdlib.h>

int ezs_greatest_common_divisor(int a, int b) {
    a = abs(a);
    b = abs(b);

    while (b != 0) {
        int tmp = b;
        b = a % b;
        a = tmp;
    }

    return a;
}

int ezs_lowest_common_multiple(int a, int b) {
    return abs(a * b) / ezs_greatest_common_divisor(a, b);
}
