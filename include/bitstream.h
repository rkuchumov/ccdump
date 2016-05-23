#ifndef BITSTREAM_H
#define BITSTREAM_H

#include <cstdlib>
#include <cstdint>

const size_t buffer_size = 2048;

size_t get_offset();

bool fill_buffer();

bool read_next();

uint32_t get_bytes(size_t n);

uint32_t get_bits(size_t n);



#endif /* end of include guard:  */

