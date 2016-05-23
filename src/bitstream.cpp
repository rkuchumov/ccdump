#include <cstdio>
#include <cassert>

#include "bitstream.h"

size_t offset = 0;

uint8_t buffer[buffer_size];

size_t bits = 0;
uint32_t get_bits(size_t n);
uint32_t get_bytes(size_t n);

size_t get_offset() {
	return offset;
}

bool fill_buffer() {
	for (size_t i = 0; i < buffer_size; i++) {
		buffer[i] = fgetc(stdin);
		if (feof(stdin))
			return false;
	}

	bits = 0;

	return !feof(stdin);
}

bool read_next() {
	for (size_t i = 1; i < buffer_size; i++)
		buffer[i - 1] = buffer[i];

	buffer[buffer_size - 1] = fgetc(stdin);

	bits = 0;

	offset++;
	return feof(stdin);
}

uint32_t get_bytes(size_t n) {
	assert(n <= 4);
	assert((bits & 0x7) == 0);

	uint32_t ret = 0;
	while (n > 0) {
		size_t byte = bits >> 3;
		ret |= buffer[byte] << ((n - 1) << 3);

		bits += 8;
		n--;
	}

	return ret;
}

uint32_t get_bits(size_t n) {
	assert(n <= 32);
	
	uint32_t ret = 0;

	while (n > 0) {
		size_t byte = bits >> 3;
		size_t bit = bits & 0x7;

		uint8_t b = (buffer[byte] & (1 << (7 - bit))) ? 1 : 0;
		ret |= b << (n - 1);

		n--;
		bits++;
	}

	return ret;
}
