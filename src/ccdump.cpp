#include <cstdio>
#include <cstdint>
#include <cassert>

#include "bitstream.h"

#include "h262.h"

int main(int argc, char *argv[])
{
	cc_block::print_cc_type = 0;
	cc_block::fold_empty    = false;

	bitstream *input;

	if (argc == 2)
		input = new bitstream(argv[1]);
	else
		input = new bitstream();

	if (!input->fill_buffer())
		return 0;

	h262 *parser = new h262(input);

	do {
		parser->parse();

		fflush(stdout);
	} while(!input->read_next());

	return 0;
}
