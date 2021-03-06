#include <cstdio>
#include <cstdint>
#include <cassert>

#include "h262.h"

h262::h262(bitstream *input_):
	input(input_)
{
	cc_block_parser = new cc_block(input);
}

h262::~h262() {
	delete cc_block_parser;
}

void h262::parse() {
	if (input->get_bytes(3) != 0x000001)
		return;

	switch (input->get_bytes(1)) {
		case 0x00:
			parse_pic();
			break;
		case 0xB2:
			cc_block_parser->parse();
			break;
		case 0xB3:
			printf("\n");
			parse_seq();
			break;
		case 0xB8:
			parse_gop();
			break;
		default:
			return;
	}

	return;
}

void h262::parse_gop() {
	printf("%lX\tGOP\t", input->tell());

	// Time code:
	uint8_t tc_drop     = input->get_bits(1);
	uint8_t tc_hours    = input->get_bits(5);
	uint8_t tc_minutes  = input->get_bits(6);
	input->get_bits(1); // Marker bit
	uint8_t tc_seconds  = input->get_bits(6);
	uint8_t tc_pictures = input->get_bits(6);

	uint8_t closed      = input->get_bits(1);
	uint8_t broken      = input->get_bits(1);

	printf("drop:%d time:%d:%d:%d:%d closed:%d broken:%d\n", tc_drop,
		tc_hours, tc_minutes, tc_seconds, tc_pictures,
		closed, broken);
}

void h262::parse_seq() {
	printf("%lX\tSEQ\t", input->tell());

	uint16_t hor_size     = input->get_bits(12);
	uint16_t ver_size     = input->get_bits(12);
	uint16_t aspect_ratio = input->get_bits(4);
	uint16_t frame_rate   = input->get_bits(4);

	printf("w:%d h:%d", hor_size, ver_size);

	printf(" ratio:");
	switch (aspect_ratio) {
		case 1: printf("1:1");    break;
		case 2: printf("4:3");    break;
		case 3: printf("16:9");   break;
		case 4: printf("2.21:1"); break;
		default: printf("other(%d)", aspect_ratio); 
	}

	printf(" rate:");
	switch (frame_rate) {
		case 1: printf("23.976"); break;
		case 2: printf("24");     break;
		case 3: printf("25");     break;
		case 4: printf("29.97");  break;
		case 5: printf("30");     break;
		case 6: printf("50");     break;
		case 7: printf("59.94");  break;
		case 8: printf("60");     break;
		default: printf("other(%d)", frame_rate);
	}

	printf("\n");
}

void h262::parse_pic() {
	printf("%lX\tPIC\t", input->tell());

	uint16_t tref = input->get_bits(10);
	uint8_t  type = input->get_bits(3);

	printf("type:");
	switch(type) {
		case 1: printf("I"); break;
		case 2: printf("P"); break;
		case 3: printf("B"); break;
		default: printf("other(%d)", type);
	}

	printf(" tref:%d", tref);

	printf("\n");
}
