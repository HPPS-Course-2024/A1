#include "numbers.h"
#include <stdlib.h>

#define RED "\033[31m"
#define YELLOW "\033[33m"
#define RESET "\033[0m"

#define ASSERT(cond, msg)                                                      \
  do {                                                                         \
    if (!(cond)) {                                                             \
      fprintf(stderr, " - %s Error:%s %s\n", RED, RESET, msg);                 \
      exit(EXIT_FAILURE);                                                      \
    }                                                                          \
  } while (0)

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

unsigned int bits8_to_int(struct bits8 x) {}
void         bits8_print(struct bits8 v) {}
struct bits8 bits8_add(struct bits8 x, struct bits8 y) {}
struct bits8 bits8_negate(struct bits8 x) {}
struct bits8 bits8_mul(struct bits8 x, struct bits8 y) {}

uint8_t from_bits8_to_uint8(struct bits8 b) {
  uint8_t result = 0;
  result |= b.b0.v;
  result |= b.b1.v << 1;
  result |= b.b2.v << 2;
  result |= b.b3.v << 3;
  result |= b.b4.v << 4;
  result |= b.b5.v << 5;
  result |= b.b6.v << 6;
  result |= b.b7.v << 7;
  return result;
}

void print_bits(struct bits8 b) {
  uint8_t result = from_bits8_to_uint8(b);
  for (int i = 0; i <= 7; i++) {
    printf("%d", (result >> (7 - i)) & 1);
  }
  printf("\n");
}

int main() {
  struct bits8 two = bits8_from_int(-1);
  print_bits(two);
}