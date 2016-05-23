#include <cstdio>
#include <cstdint>
#include <cassert>

#include "bitstream.h"

#include "h262.h"

int main()
{
	if (!fill_buffer())
		return 0;

	fprintf(stderr, "here\n");

	do {
		parse_h264();

		fflush(stdout);
	} while(!read_next());

	return 0;
}
