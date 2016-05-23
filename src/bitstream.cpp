#include <cassert>

#include "bitstream.h"

bitstream::bitstream() {
	fp = stdin;

	buffer = new uint8_t[buffer_size];
	offset = 0;
	bits = 0;
}

bitstream::bitstream(const char *input) {
	if (input)
		fp = fopen(input, "rb");
	else
		fp = stdin;

	if (fp == nullptr)
		throw "ahaha";

	offset = 0;
	bits = 0;
	buffer = new uint8_t[buffer_size];
}

bitstream::~bitstream() {
	delete []buffer;
}

size_t bitstream::tell() {
	assert(fp != nullptr);

	return offset;
}

bool bitstream::fill_buffer() {
	assert(fp != nullptr);
	assert(buffer != nullptr);

	for (size_t i = 0; i < buffer_size; i++) {
		buffer[i] = fgetc(fp);
		if (feof(fp))
			return false;
	}

	bits = 0;

	return !feof(fp);
}

bool bitstream::read_next() {
	assert(fp != nullptr);
	assert(buffer != nullptr);

	for (size_t i = 1; i < buffer_size; i++)
		buffer[i - 1] = buffer[i];

	buffer[buffer_size - 1] = fgetc(fp);

	bits = 0;

	offset++;
	return feof(fp);
}

uint32_t bitstream::get_bytes(size_t n) {
	assert(n <= 4);
	assert((bits & 0x7) == 0);
	assert(buffer != nullptr);

	uint32_t ret = 0;
	while (n > 0) {
		size_t byte = bits >> 3;
		if (byte >= buffer_size)
			throw "Buffer overflow";

		ret |= buffer[byte] << ((n - 1) << 3);

		bits += 8;
		n--;
	}

	return ret;
}

uint32_t bitstream::get_bits(size_t n) {
	assert(n <= 32);
	assert(buffer != nullptr);
	
	uint32_t ret = 0;

	while (n > 0) {
		size_t byte = bits >> 3;
		size_t bit = bits & 0x7;

		if (byte >= buffer_size)
			throw "Buffer overflow";

		uint8_t b = (buffer[byte] & (1 << (7 - bit))) ? 1 : 0;
		ret |= b << (n - 1);

		n--;
		bits++;
	}

	return ret;
}
