#include "numbers.h"
#include "comparators.h"
#include "printers.h"
#include <string.h>

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
  ASSERT((uint8_t)x <= 255, "Value is too large for 8 bits");

  struct bits8 result; // x = 10, 0b1010
  result.b0.v = x & 1;
  result.b1.v = x >> 1 & 1;
  result.b2.v = x >> 2 & 1;
  result.b3.v = x >> 3 & 1;
  result.b4.v = x >> 4 & 1;
  result.b5.v = x >> 5 & 1;
  result.b6.v = x >> 6 & 1;
  result.b7.v = x >> 7 & 1;
  return result;
}

unsigned int bits8_to_int(struct bits8 x) {
  unsigned int result = 0;
  result |= bit_to_int(x.b0);

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

void add_helper(int* carry, struct bit* bit, bool xv, bool yv) {
  if (xv && yv) {
    bit->v |= *carry;
    *carry |= !(*carry);
  } else if (xv || yv) {
    bit->v |= !(*carry);
  } else {
    bit->v |= *carry;
    *carry = 0;
  }
}

typedef struct {
  int*        c;
  struct bit* b;
  bool        xv, yv;
} bit_operations_queue_t;

void populate_bit_queue(bit_operations_queue_t queue[], int* carry,
                        struct bits8* result, struct bits8 x, struct bits8 y) {
  bit_operations_queue_t temp[] = {{carry, &result->b0, x.b0.v, y.b0.v},
                                   {carry, &result->b1, x.b1.v, y.b1.v},
                                   {carry, &result->b2, x.b2.v, y.b2.v},
                                   {carry, &result->b3, x.b3.v, y.b3.v},
                                   {carry, &result->b4, x.b4.v, y.b4.v},
                                   {carry, &result->b5, x.b5.v, y.b5.v},
                                   {carry, &result->b6, x.b6.v, y.b6.v},
                                   {carry, &result->b7, x.b7.v, y.b7.v}};
  memcpy(queue, temp, sizeof(temp));
}

struct bits8 bits8_add(struct bits8 x, struct bits8 y) {
  int          carry  = 0;
  struct bits8 result = {0};

  bit_operations_queue_t queue[8];
  populate_bit_queue(queue, &carry, &result, x, y);

  for (int i = 0; i < 8; i++) {
    add_helper(queue[i].c, queue[i].b, queue[i].xv, queue[i].yv);
  }
  return result;
}

struct bits8 bits8_negate(struct bits8 x) {
  struct bits8 result = {0};
  result.b0.v         = !x.b0.v;
  result.b1.v         = !x.b1.v;
  result.b2.v         = !x.b2.v;
  result.b3.v         = !x.b3.v;
  result.b4.v         = !x.b4.v;
  result.b5.v         = !x.b5.v;
  result.b6.v         = !x.b6.v;
  result.b7.v         = !x.b7.v;
  return bits8_add(result, bits8_from_int(1));
}

struct bits8 bits8_mul(struct bits8 x, struct bits8 y) {
  uint8_t z = 0;

  uint8_t x_bits_as_int = bits8_to_int(x);
  uint8_t y_bits_as_int = bits8_to_int(y);

  for (int i = 0; i < 8; i++) {
    if (y_bits_as_int >> i & 1) {
      x = bits8_from_int(x_bits_as_int << i);
      z += bits8_to_int(x);
    }
  }
  return bits8_from_int(z);
}
