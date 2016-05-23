#include <cstdio>
#include <cstdint>

#include "bitstream.h"
#include "608.h"

#include "cc_block.h"

void parse_atsc() {
	if (get_bytes(4) != 0x47413934) // ATSC Identifier
		return;
	if (get_bytes(1) != 0x03) // CC Data
		return;

	get_bits(1); // Reserved
	get_bits(1); // Process CC Data
	get_bits(1); // Additional data
	uint8_t cc_count = get_bits(5);
	get_bytes(1); // Reserved

	for (uint8_t i = 0; i < cc_count; i++) {
		get_bits(5); // Marker bits
		uint8_t valid     = get_bits(1);
		uint8_t cc_type   = get_bits(2);
		uint8_t cc_data_1 = get_bytes(1) & 0x7F; // Without parity
		uint8_t cc_data_2 = get_bytes(1) & 0x7F; // Without parity

		// if (cc_type != print_cc_type)
		// 	continue;

		printf("%lX\tCC\t", get_offset());
		printf("v:%d t:%d ", valid, cc_type);
		printf("[%02X;%02X] ", cc_data_1, cc_data_2);

		parse_608(cc_data_1, cc_data_2);

		printf("\n");
	}
}
