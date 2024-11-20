#include "comparators.h"
#include "numbers.h"
#include "printers.h"
#include <stdint.h>
#include <stdio.h>

void print_header() {
  printf("\n%s%-3s %-25s | %-14s %-14s %s%s\n", BOLD, "No", "Description",
         "Expected", "Got", "Result", RESET);
}

typedef int (*Comparator)(void* expected, void* got);
typedef void (*Printer)(void* value, char* buffer, size_t buffer_size);

typedef struct {
  int passed;
  int failed;
} TestResult;

int assert_test(uint8_t test_case, const char* description, void* expected,
                void* got, Comparator compare, Printer print_expected,
                Printer print_got, TestResult* result) {
  const char* result_icon;
  const char* result_color;
  int         succeeded = compare(expected, got);

  if (succeeded) {
    result_icon  = CHECKMARK;
    result_color = GREEN;
    result->passed++;
  } else {
    result_icon  = CROSSMARK;
    result_color = RED;
    result->failed++;
  }

  char expected_str[32];
  char got_str[32];

  print_expected(expected, expected_str, sizeof(expected_str));
  print_got(got, got_str, sizeof(got_str));

  char parenthesized_description[40];
  snprintf(parenthesized_description, sizeof(parenthesized_description), "(%s)",
           description);

  printf("%-3u %-25s | %-14s %-14s %s%s%s\n", test_case,
         parenthesized_description, expected_str, got_str, result_color,
         result_icon, RESET);
  return !succeeded;
}

int test_bits8_from_int(int* counter, TestResult* result) {
  int success = 0;

  struct bits8 got;
  uint8_t      expected;

  expected = 81;
  got      = bits8_from_int(expected);
  success |= assert_test((*counter)++, "Convert 81", &expected, &got,
                         &uint8_bits8_comparator, &uint8_to_base2,
                         &bits8_to_base2, result);

  expected = 255;
  got      = bits8_from_int(expected);
  success |= assert_test((*counter)++, "Convert 255", &expected, &got,
                         &uint8_bits8_comparator, &uint8_to_base2,
                         &bits8_to_base2, result);

  expected = 0;
  got      = bits8_from_int(expected);
  success |= assert_test((*counter)++, "Convert 0", &expected, &got,
                         &uint8_bits8_comparator, &uint8_to_base2,
                         &bits8_to_base2, result);

  expected = 127;
  got      = bits8_from_int(expected);
  success |= assert_test((*counter)++, "Convert 127", &expected, &got,
                         &uint8_bits8_comparator, &uint8_to_base2,
                         &bits8_to_base2, result);

  expected = -1;
  got      = bits8_from_int(expected);
  success |= assert_test((*counter)++, "Convert -1", &expected, &got,
                         &uint8_bits8_comparator, &uint8_to_base2,
                         &bits8_to_base2, result);

  expected = -128;
  got      = bits8_from_int(expected);
  success |= assert_test((*counter)++, "Convert -128", &expected, &got,
                         &uint8_bits8_comparator, &uint8_to_base2,
                         &bits8_to_base2, result);

  return success;
}

// Testing upperbound, lowerbound and something that isn't symmetric, to check
// LSB and MSB is right place.
int test_bits8_to_int(int* counter, TestResult* result) {
  int success = 0;

  uint8_t expected, got;

  expected = 0b10100010; // Decimal 162
  got      = bits8_to_int(bits8_init(1, 0, 1, 0, 0, 0, 1, 0));
  success |= assert_test((*counter)++, "Convert 0x10100010", &expected, &got,
                         &uint8_uint8_comparator, &to_uint8_base10,
                         &to_uint8_base10, result);

  expected = 0b10101010; // Decimal 170
  got      = bits8_to_int(bits8_init(1, 0, 1, 0, 1, 0, 1, 0));
  success |= assert_test((*counter)++, "Convert 0x10101010", &expected, &got,
                         &uint8_uint8_comparator, &to_uint8_base10,
                         &to_uint8_base10, result);

  expected = 0b01010101; // Decimal 85
  got      = bits8_to_int(bits8_init(0, 1, 0, 1, 0, 1, 0, 1));
  success |= assert_test((*counter)++, "Convert 0x01010101", &expected, &got,
                         &uint8_uint8_comparator, &to_uint8_base10,
                         &to_uint8_base10, result);

  expected = 0b00000001; // Decimal 1
  got      = bits8_to_int(bits8_init(0, 0, 0, 0, 0, 0, 0, 1));
  success |= assert_test((*counter)++, "Convert 0x00000001", &expected, &got,
                         &uint8_uint8_comparator, &to_uint8_base10,
                         &to_uint8_base10, result);

  expected = 0b10000000; // Decimal 128
  got      = bits8_to_int(bits8_init(1, 0, 0, 0, 0, 0, 0, 0));
  success |= assert_test((*counter)++, "Convert 0x10000000", &expected, &got,
                         &uint8_uint8_comparator, &to_uint8_base10,
                         &to_uint8_base10, result);

  expected = 255; // Binary 0b11111111
  got      = bits8_to_int(bits8_init(1, 1, 1, 1, 1, 1, 1, 1));
  success |= assert_test((*counter)++, "Convert 0x11111111", &expected, &got,
                         &uint8_uint8_comparator, &to_uint8_base10,
                         &to_uint8_base10, result);

  expected = 0; // Binary 0b00000000
  got      = bits8_to_int(bits8_init(0, 0, 0, 0, 0, 0, 0, 0));
  success |= assert_test((*counter)++, "Convert 0x00000000", &expected, &got,
                         &uint8_uint8_comparator, &to_uint8_base10,
                         &to_uint8_base10, result);

  return success;
}

int test_bits8_addition(int* counter, TestResult* result) {
  int success = 0;

  struct bits8 x, y;

  uint8_t expected;
  uint8_t got;

  // Test cases for powers of 2 addition
  for (int i = 1; i < 8; i++) {
    expected = 1 << i; // Same as 2^i
    x        = bits8_from_int(1 << (i - 1));
    got      = bits8_to_int(bits8_add(x, x));

    char description[32];
    snprintf(description, sizeof(description), "%d + %d", 1 << (i - 1),
             1 << (i - 1));
    success |= assert_test((*counter)++, description, &expected, &got,
                           &uint8_uint8_comparator, &uint8_to_base2,
                           &uint8_to_base2, result);
  }

  // Test cases for fixed additions
  for (int i = 100; i < 104; i++) {
    expected = 2 * i;
    x        = bits8_from_int(i);
    y        = bits8_from_int(i);
    got      = bits8_to_int(bits8_add(x, y));

    char description[32];
    snprintf(description, sizeof(description), "%d + %d", i, i);
    success |= assert_test((*counter)++, description, &expected, &got,
                           &uint8_uint8_comparator, &to_uint8_base10,
                           &to_uint8_base10, result);
  }

  expected = 50;
  got      = bits8_to_int(bits8_add(bits8_from_int(100), bits8_from_int(-50)));
  success |= assert_test((*counter)++, "100 + -50", &expected, &got,
                         &uint8_uint8_comparator, to_int8_base10,
                         to_int8_base10, result);

  expected = -8;
  got      = bits8_to_int(bits8_add(bits8_from_int(-5), bits8_from_int(-3)));
  success |= assert_test((*counter)++, "-5 + -3", &expected, &got,
                         &uint8_uint8_comparator, to_int8_base10,
                         to_int8_base10, result);

  return success;
}

int test_bits8_negate(int* counter, TestResult* result) {
  int success = 0;

  uint8_t      expected;
  struct bits8 x, got;

  // Test case for negating -1 to 1
  expected = 1;
  x        = bits8_from_int(-1);
  got      = bits8_negate(x);
  success |= assert_test((*counter)++, "Negate -1", &expected, &got,
                         &uint8_bits8_comparator, &to_int8_base10,
                         &bits8_to_base10_signed, result);

  // Test case for negating 1 to -1
  expected = -1;
  x        = bits8_from_int(1);
  got      = bits8_negate(x);
  success |= assert_test((*counter)++, "Negate 1", &expected, &got,
                         &uint8_bits8_comparator, &to_int8_base10,
                         &bits8_to_base10_signed, result);

  // Test case for negating 0 to 0
  expected = 0;
  x        = bits8_from_int(0);
  got      = bits8_negate(x);
  success |= assert_test((*counter)++, "Negate 0", &expected, &got,
                         &uint8_bits8_comparator, &to_int8_base10,
                         &bits8_to_base10_signed, result);

  // Test case for double negation
  expected = -1;
  x        = bits8_from_int(-1);
  got      = bits8_negate(bits8_negate(x));
  success |= assert_test((*counter)++, "Double negate -1", &expected, &got,
                         &uint8_bits8_comparator, &to_int8_base10,
                         &bits8_to_base10_signed, result);

  // Test cases for negating alternating positive and negative values
  for (int i = 1; i <= 10; i++) {
    expected = i % 2 ? i * 20 : -i * 10;
    x        = bits8_from_int(-expected);
    got      = bits8_negate(x);

    char description[32];
    snprintf(description, sizeof(description), "Negate %d",
             (int8_t)(-expected));
    success |= assert_test((*counter)++, description, &expected, &got,
                           &uint8_bits8_comparator, &to_int8_base10,
                           &bits8_to_base10_signed, result);
  }

  return success;
}

int test_bits8_mul(int* counter, TestResult* result) {
  int          success = 0;
  uint8_t      expected, got;
  struct bits8 x, y;

  // Test case: 5 * 20 = 100
  x        = bits8_from_int(5);
  y        = bits8_from_int(20);
  expected = 100;
  got      = bits8_to_int(bits8_mul(x, y));
  success |= assert_test((*counter)++, "5 * 20", &expected, &got,
                         &uint8_uint8_comparator, &to_uint8_base10,
                         &to_uint8_base10, result);

  // Test cases for powers of 2 multiplication
  for (int i = 1; i < 8; i++) {
    expected = 1 << i;
    x        = bits8_from_int(1 << (i - 1));
    got      = bits8_to_int(bits8_mul(x, bits8_from_int(2)));

    char description[32];
    snprintf(description, sizeof(description), "%d * 2", 1 << (i - 1));
    success |= assert_test((*counter)++, description, &expected, &got,
                           &uint8_uint8_comparator, &to_uint8_base10,
                           &to_uint8_base10, result);
  }

  struct {
    int x, y;
  } cases[] = {{-5, 20}, {5, -20}, {-5, -20}, {5, -1}, {-5, -1}};

  for (int i = 0; i < sizeof(cases) / sizeof(cases[0]); i++) {
    expected          = cases[i].x * cases[i].y;
    struct bits8 val1 = bits8_from_int(cases[i].x);
    struct bits8 val2 = bits8_from_int(cases[i].y);
    got               = bits8_to_int(bits8_mul(val1, val2));

    char description[32];
    snprintf(description, sizeof(description), "%d * %d", cases[i].x,
             cases[i].y);

    Printer printer =
        (cases[i].x | cases[i].y) < 0 ? &to_int8_base10 : &to_uint8_base10;

    success |= assert_test((*counter)++, description, &expected, &got,
                           &uint8_uint8_comparator, printer, printer, result);
  }
  return success;
}

int main() {
  struct {
    int (*func_ptr)(int*, TestResult*);
    const char* name;
  } test_cases[] = {{test_bits8_from_int, "bits8_from_int"},
                    {test_bits8_to_int, "bits8_to_int"},
                    {test_bits8_addition, "bits8_addition"},
                    {test_bits8_negate, "bits8_negate"},
                    {test_bits8_mul, "bits8_mul"}};

  int        test_counter = 1;
  TestResult result       = {0, 0};

  for (int i = 0; i < sizeof(test_cases) / sizeof(test_cases[0]); i++) {
    print_header();
    printf("%s:\n", test_cases[i].name);

    int   res    = test_cases[i].func_ptr(&test_counter, &result);
    char* color  = res == 0 ? GREEN : RED;
    char* result = res == 0 ? "PASSED" : "FAILED";

    printf("%s %s%s%s:\n\n", test_cases[i].name, color, result, RESET);

    char msg_buffer[100];
    snprintf(msg_buffer, sizeof(msg_buffer), "Test failed for %s",
             test_cases[i].name);
    ASSERT(res == 0, msg_buffer);
  }
  printf("Summary: ");
  printf("%s%d tests passed%s, ", GREEN, result.passed, RESET);
  printf("%s%d tests failed%s\n\n", RED, result.failed, RESET);
}