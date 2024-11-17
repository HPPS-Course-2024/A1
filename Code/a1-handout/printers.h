#ifndef PRINTERS_H
#define PRINTERS_H

#include "numbers.h" // Include numbers.h to use struct bits8
#include <stdint.h>
#include <stdio.h>

void uint8_to_base10(void* value, char* buffer, size_t buffer_size);
void uint8_to_base2(void* value, char* buffer, size_t buffer_size);
void bits8_to_base2(void* value, char* buffer, size_t buffer_size);
void bits8_to_base10(void* value, char* buffer, size_t buffer_size);

#endif
