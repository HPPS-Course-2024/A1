#ifndef BITS_H
#define BITS_H

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

struct bit {
  bool v;
};

struct bit   bit_from_int(unsigned int x);
unsigned int bit_to_int(struct bit b);
void         bit_print(struct bit b);
struct bit   bit_not(struct bit a);
struct bit   bit_and(struct bit a, struct bit b);
struct bit   bit_or(struct bit a, struct bit b);
struct bit   bit_xor(struct bit a, struct bit b);

#endif
