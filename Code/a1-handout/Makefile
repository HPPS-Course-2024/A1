CFLAGS=-Wall -Wextra -pedantic -std=c99

test_bits: test_bits.c bits.h
	gcc -o test_bits test_bits.c $(CFLAGS)

HEADERS = bits.h numbers.h comparators.h printers.h
SOURCES = test_numbers.c numbers.c bits.c comparators.c printers.c
TARGET = test_numbers

$(TARGET): $(SOURCES) $(HEADERS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES)

clean:
	rm -f test_bits test_numbers
