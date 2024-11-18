#ifndef NUMBERS_H
#define NUMBERS_H

#include "bits.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

struct bits8 {
  struct bit b0; // Least significant bit
  struct bit b1;
  struct bit b2;
  struct bit b3;
  struct bit b4;
  struct bit b5;
  struct bit b6;
  struct bit b7;
};

struct bits8 bits8_from_int(unsigned int x);
unsigned int bits8_to_int(struct bits8 x);
void         bits8_print(struct bits8 v);
struct bits8 bits8_add(struct bits8 x, struct bits8 y);
struct bits8 bits8_negate(struct bits8 x);
struct bits8 bits8_mul(struct bits8 x, struct bits8 y);

// Our own helper functions
void         bits8_to_ascii(struct bits8 x, char* buffer);
struct bits8 bits8_init(bool b0, bool b1, bool b2, bool b3, bool b4, bool b5,
                        bool b6, bool b7);
#define RED "\033[31m"
#define GREEN "\033[32m"
#define RESET "\033[0m"
#define BOLD "\033[1m"

#define CHECKMARK "\u2714"
#define CROSSMARK "\u2718"

#define INFO(msg)                                                              \
  do {                                                                         \
    fprintf(stdout, " %sℹ️  Info:%s %s\n", GREEN, RESET, msg);                  \
  } while (0)

#define ASSERT(cond, msg)                                                      \
  do {                                                                         \
    if (!(cond)) {                                                             \
      fprintf(stderr, " - %s Error:%s %s\n", RED, RESET, msg);                 \
    }                                                                          \
  } while (0)

#endif
