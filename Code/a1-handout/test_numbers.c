#include "comparators.h"
#include "numbers.h"
#include "printers.h"
#include <stdint.h>
#include <stdio.h>

void print_header() {
  printf("%-12s  %-12s  %-12s  %s\n", "Test Case", "Expected", "Got", "Result");
}

typedef int (*Comparator)(void* expected, void* got);
typedef void (*Printer)(void* value, char* buffer, size_t buffer_size);

int assert_test(uint8_t test_case, void* expected, void* got,
                Comparator compare, Printer print_expected, Printer print_got) {
  const char* result_icon;
  const char* result_color;
  int         result = 0;
  if (compare(expected, got)) {
    result_icon  = CHECKMARK;
    result_color = GREEN;
    result       = 0;
  } else {
    result_icon  = CROSSMARK;
    result_color = RED;
    result       = 1;
  }

  char expected_str[32];
  char got_str[32];

  print_expected(expected, expected_str, sizeof(expected_str));
  print_got(got, got_str, sizeof(got_str));

  printf("%-12u  %-12s  %-12s  %s%s%s\n", test_case, expected_str, got_str,
         result_color, result_icon, RESET);
  return result;
}

int test_bits8_from_int(int* counter) {
  int success = 0;

  struct bits8 got;
  uint8_t      expected;

  expected = 0b1010001;
  got      = bits8_from_int(expected);
  success |= assert_test((*counter)++, &expected, &got, &uint8_bits8_comparator,
                         &uint8_to_base2, &bits8_to_base2);

  expected = 0b11111111;
  got      = bits8_from_int(expected);
  success |= assert_test((*counter)++, &expected, &got, &uint8_bits8_comparator,
                         &uint8_to_base2, &bits8_to_base2);

  expected = 255;
  got      = bits8_from_int(expected);
  success |= assert_test((*counter)++, &expected, &got, &uint8_bits8_comparator,
                         &uint8_to_base10, &bits8_to_base2);

  expected = 0;
  got      = bits8_from_int(expected);
  success |= assert_test((*counter)++, &expected, &got, &uint8_bits8_comparator,
                         &uint8_to_base10, &bits8_to_base2);

  expected = 127;
  got      = bits8_from_int(expected);
  success |= assert_test((*counter)++, &expected, &got, &uint8_bits8_comparator,
                         &uint8_to_base10, &bits8_to_base10);
  return success;
}

// Testing upperbound, lowerbound and something that isn't symmetric, to check
// LSB and MSB is right place.
int test_bits8_to_int(int* counter) {
  int success = 0;

  uint8_t expected, got;
  expected = 0b10100010; // Decimal 162
  got      = bits8_to_int(bits8_init(1, 0, 1, 0, 0, 0, 1, 0));
  success |= assert_test((*counter)++, &expected, &got, &uint8_uint8_comparator,
                         &uint8_to_base2, &uint8_to_base2);

  expected = 0b10101010; // Decimal 170
  got      = bits8_to_int(bits8_init(1, 0, 1, 0, 1, 0, 1, 0));
  success |= assert_test((*counter)++, &expected, &got, &uint8_uint8_comparator,
                         &uint8_to_base2, &uint8_to_base2);

  expected = 0b01010101; // Decimal 85
  got      = bits8_to_int(bits8_init(0, 1, 0, 1, 0, 1, 0, 1));
  success |= assert_test((*counter)++, &expected, &got, &uint8_uint8_comparator,
                         &uint8_to_base2, &uint8_to_base2);

  expected = 0b00000001; // Decimal 1
  got      = bits8_to_int(bits8_init(0, 0, 0, 0, 0, 0, 0, 1));
  success |= assert_test((*counter)++, &expected, &got, &uint8_uint8_comparator,
                         &uint8_to_base2, &uint8_to_base2);

  expected = 0b10000000; // Decimal 128
  got      = bits8_to_int(bits8_init(1, 0, 0, 0, 0, 0, 0, 0));
  success |= assert_test((*counter)++, &expected, &got, &uint8_uint8_comparator,
                         &uint8_to_base2, &uint8_to_base2);

  expected = 255; // Binary 0b11111111
  got      = bits8_to_int(bits8_init(1, 1, 1, 1, 1, 1, 1, 1));
  success |= assert_test((*counter)++, &expected, &got, &uint8_uint8_comparator,
                         &uint8_to_base2, &uint8_to_base2);

  expected = 0; // Binary 0b00000000
  got      = bits8_to_int(bits8_init(0, 0, 0, 0, 0, 0, 0, 0));
  success |= assert_test((*counter)++, &expected, &got, &uint8_uint8_comparator,
                         &uint8_to_base2, &uint8_to_base2);
  return success;
}

int test_bits8_addition(int* counter) {
  // Do something like this and use add 1 and see if its works for like 10
  // cases, would be a nice way of testing, and this is the last more complex
  // case, then just a few base cases above.
  //  for (int i = 0; i < 10; ++i) {
  //    expected = 0b10101010; // Decimal 170
  //    got      = bits8_to_int(bits8_init(1, 0, 1, 0, 1, 0, 1, 0));
  //    success |=
  //        assert_test((*counter)++, &expected, &got, &uint8_uint8_comparator,
  //                    &uint8_to_base2, &uint8_to_base2);
  //  }
}

int main() {
  print_header();

  struct {
    int (*func_ptr)(int*);
    const char* name;
  } test_cases[] = {{test_bits8_from_int, "bits8_from_int"},
                    {test_bits8_to_int, "bits8_to_int"}};

  int test_counter = 1;

  for (int i = 0; i < sizeof(test_cases) / sizeof(test_cases[0]); i++) {
    printf("%s:\n", test_cases[i].name);

    int   res    = test_cases[i].func_ptr(&test_counter);
    char* color  = res == 0 ? GREEN : RED;
    char* result = res == 0 ? "PASSED" : "FAILED";

    char msg_buffer[100];
    snprintf(msg_buffer, sizeof(msg_buffer), "Test failed for %s",
             test_cases[i].name);
    ASSERT(res == 0, msg_buffer);
  }
}