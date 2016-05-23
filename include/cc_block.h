#ifndef CC_BLOCK_H
#define CC_BLOCK_H

#include <cstdint>

#include "bitstream.h"
#include "608.h"

class cc_block
{
public:
	cc_block(bitstream *input);
	~cc_block();

	void parse();

	static uint8_t print_cc_type;
	static bool fold_empty;

private:
	void parse_atsc();

	bitstream *input;

	cc_608 *cc_608_parser;

	size_t folded_empty_count;
};


#endif /* end of include guard: CC_BLOCK_H */

