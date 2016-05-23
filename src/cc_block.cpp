#include <cstdio>
#include <cstdint>

#include "cc_block.h"

uint8_t cc_block::print_cc_type = 0;
bool cc_block::fold_empty = false;

cc_block::cc_block(bitstream *input_):
	input(input_)
{
	cc_608_parser = new cc_608();
}

cc_block::~cc_block() {
	delete cc_608_parser;
}

void cc_block::parse() {
	parse_atsc();
}

void cc_block::parse_atsc() {
	if (input->get_bytes(4) != 0x47413934) // ATSC Identifier
		return;
	if (input->get_bytes(1) != 0x03) // CC Data
		return;

	input->get_bits(1); // Reserved
	input->get_bits(1); // Process CC Data
	input->get_bits(1); // Additional data
	uint8_t cc_count = input->get_bits(5);
	input->get_bytes(1); // Reserved

	for (uint8_t i = 0; i < cc_count; i++) {
		input->get_bits(5); // Marker bits
		uint8_t valid     = input->get_bits(1);
		uint8_t cc_type   = input->get_bits(2);
		uint8_t cc_data_1 = input->get_bytes(1) & 0x7F; // Without parity
		uint8_t cc_data_2 = input->get_bytes(1) & 0x7F; // Without parity

		if (cc_type != print_cc_type)
			continue;

		if (fold_empty && cc_data_1 == 0 && cc_data_2 == 0) {
			folded_empty_count++;
			continue;
		}

		if (folded_empty_count > 0)
			printf("------\tCC\tSkipped %zd empty block(s)\n", folded_empty_count);
		folded_empty_count = 0;

		printf("%lX\tCC\t", input->tell());
		printf("v:%d t:%d ", valid, cc_type);
		printf("[%02X;%02X] ", cc_data_1, cc_data_2);

		cc_608_parser->parse(cc_data_1, cc_data_2);

		printf("\n");
	}
}
