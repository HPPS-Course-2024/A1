#include "numbers.h"
#include "comparators.h"
#include "printers.h"

struct bits8 bits8_init(bool b7, bool b6, bool b5, bool b4, bool b3, bool b2,
                        bool b1, bool b0) {
  struct bits8 result;
  result.b7.v = b7; // Most significant bit first (MSB)
  result.b6.v = b6;
  result.b5.v = b5;
  result.b4.v = b4;
  result.b3.v = b3;
  result.b2.v = b2;
  result.b1.v = b1;
  result.b0.v = b0; // Least significant bit last (LSB)
  return result;
}

struct bits8 bits8_from_int(unsigned int x) {
  ASSERT(x <= 255, "Value is too large for 8 bits");
  struct bits8 result;
  result.b0 = bit_from_int(x & 1);
  result.b1 = bit_from_int(x >> 1 & 1);
  result.b2 = bit_from_int(x >> 2 & 1);
  result.b3 = bit_from_int(x >> 3 & 1);
  result.b4 = bit_from_int(x >> 4 & 1);
  result.b5 = bit_from_int(x >> 5 & 1);
  result.b6 = bit_from_int(x >> 6 & 1);
  result.b7 = bit_from_int(x >> 7 & 1);
  return result;
}

unsigned int bits8_to_int(struct bits8 x) {
  unsigned int result = 0;
  result |= x.b0.v;      // LSB,  0 => 0,   1 => 1
  result |= x.b1.v << 1; //       0 => 00,  1 => 10
  result |= x.b2.v << 2; //       0 => 000, 1 => 100
  result |= x.b3.v << 3;
  result |= x.b4.v << 4;
  result |= x.b5.v << 5;
  result |= x.b6.v << 6;
  result |= x.b7.v << 7;
  return result;
}

void bits8_print(struct bits8 v) {
  uint8_t result = bits8_to_int(v);
  for (int i = 0; i <= 7; i++) {
    printf("%d", (result >> (7 - i)) & 1);
  }
  printf("\n");
}

void bits8_to_ascii(struct bits8 x, char* buffer) {
  uint8_t value = bits8_to_int(x);
  for (int i = 7; i >= 0; i--) {
    buffer[7 - i] = (value >> i) & 1 ? '1' : '0';
  }
  buffer[8] = '\0';
}

struct bits8 bits8_add(struct bits8 x, struct bits8 y) {}
struct bits8 bits8_negate(struct bits8 x) {}
struct bits8 bits8_mul(struct bits8 x, struct bits8 y) {}

// test_numbers.c
