#include "comparators.h"

int uint8_bits8_comparator(void* expected, void* got) {
  uint8_t*      exp          = (uint8_t*)expected;
  struct bits8* actual       = (struct bits8*)got;
  uint8_t       actual_value = bits8_to_int(*actual);
  return (*exp == actual_value);
}

int uint8_uint8_comparator(void* expected, void* got) {
  uint8_t* exp = (uint8_t*)expected;
  uint8_t* act = (uint8_t*)got;
  return (*exp == *act);
}