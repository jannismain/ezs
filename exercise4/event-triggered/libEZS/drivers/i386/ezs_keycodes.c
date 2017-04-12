
#include "ezs_keycodes.h"
#include <assert.h>
#include <stdio.h>

#define NUMBER_OF_KEYCODES 89
unsigned char char_of_key[] = {
  0, 0, '1', '2', '3', '4', '5', '6', '7', '8',
  '9', '0', 225, 39, '\b', 0, 'q', 'w', 'e', 'r',
  't', 'z', 'u', 'i', 'o', 'p', 129, '+', '\n', 0,
  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', 148,
  132, '^', 0, '#', 'y', 'x', 'c', 'v', 'b', 'n',
  'm', ',', '.', '-', 0, '*', 0, ' ', 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, '-', 0, 0, 0, '+', 0,
  0, 0, 0, 0, 0, 0, '<', 0, 0 };

unsigned char ezs_get_char_from_keycode(unsigned char code) {
  assert(code < NUMBER_OF_KEYCODES);

  return char_of_key[code];
}
