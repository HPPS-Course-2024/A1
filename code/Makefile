CFLAGS=-Wall -Wextra -pedantic -std=c99
HEADERS = bits.h numbers.h comparators.h printers.h
SOURCES = test_numbers.c numbers.c bits.c comparators.c printers.c
TARGET = test_numbers

all: test_bits test_numbers

test_bits: test_bits.c bits.c bits.h
	$(CC) $(CFLAGS) -o test_bits test_bits.c bits.c

$(TARGET): $(SOURCES) $(HEADERS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES)

clean:
	rm -f test_bits test_numbers
