#include "numbers.h"

struct bits8 bits8_from_int(unsigned int x) {
  // An integer is 4 bytes =>
  return (struct bits8){
      .b0 = 0, .b1 = 1, .b2 = 0, .b3 = 0, .b4 = 0, .b5 = 0, .b6 = 0, .b7 = 0};
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
  struct bits8 two = bits8_from_int(2);
  print_bits(two);
}