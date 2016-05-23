#ifndef H262_H
#define H262_H

#include "cc_block.h"
#include "bitstream.h"

class h262
{
public:
	h262(bitstream *input);
	~h262();

	void parse();

private:
	void parse_gop();
	void parse_seq();
	void parse_pic();

	bitstream *input;

	cc_block *cc_block_parser;
};

#endif /* end of include guard: H262_H */

