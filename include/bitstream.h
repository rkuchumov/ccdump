#ifndef BITSTREAM_H
#define BITSTREAM_H

#include <cstdlib>
#include <cstdint>
#include <cstdio>

class bitstream
{
public:
	bitstream (const char *input);
	bitstream ();

	~bitstream ();

	bool fill_buffer();

	size_t tell();

	bool read_next();

	uint32_t get_bytes(size_t n);
	uint32_t get_bits(size_t n);

private:
	const size_t buffer_size = 2048;
	uint8_t *buffer;

	size_t offset;
	size_t bits;

	FILE *fp;
};

#endif /* end of include guard:  */

