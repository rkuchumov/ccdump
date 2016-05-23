#include <cstdio>
#include <cstdint>
#include <cassert>

const uint8_t print_cc_type = 0;

const size_t buffer_size = 2048;

uint8_t buffer[buffer_size];

size_t offset = 0;
bool read_next();

size_t bits = 0;
uint32_t get_bits(size_t n);
uint32_t get_bytes(size_t n);

void parse_gop();
void parse_seq();
void parse_pic();
void parse_usr();
void parse_608(uint8_t hi, uint8_t lo);
void parse_608_command(uint8_t hi, uint8_t lo);

int main()
{
	for (size_t i = 0; i < buffer_size; i++) {
		if (feof(stdin))
			return 0;
		buffer[i] = fgetc(stdin);
	}

	do {
		if (get_bytes(3) != 0x000001)
			continue;

		switch (get_bytes(1)) {
			case 0x00:
				parse_pic();
				break;
			case 0xB2:
				parse_usr();
				break;
			case 0xB3:
				printf("\n");
				parse_seq();
				break;
			case 0xB8:
				parse_gop();
				break;

			default:
				continue;
		}

	} while(!read_next());

	return 0;
}

void parse_gop() {
	printf("%lX\tGOP\t", offset);

	// Time code:
	uint8_t tc_drop     = get_bits(1);
	uint8_t tc_hours    = get_bits(5);
	uint8_t tc_minutes  = get_bits(6);
	get_bits(1); // Marker bit
	uint8_t tc_seconds  = get_bits(6);
	uint8_t tc_pictures = get_bits(6);

	uint8_t closed      = get_bits(1);
	uint8_t broken      = get_bits(1);

	printf("drop:%d time:%d:%d:%d:%d closed:%d broken:%d\n", tc_drop,
		tc_hours, tc_minutes, tc_seconds, tc_pictures,
		closed, broken);
}

void parse_seq() {
	printf("%lX\tSEQ\t", offset);

	uint16_t hor_size     = get_bits(12);
	uint16_t ver_size     = get_bits(12);
	uint16_t aspect_ratio = get_bits(4);
	uint16_t frame_rate   = get_bits(4);

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

void parse_pic() {
	printf("%lX\tPIC\t", offset);

	uint16_t tref = get_bits(10);
	uint8_t  type = get_bits(3);

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

void parse_usr() {
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

		if (cc_type != print_cc_type)
			continue;

		printf("%lX\tCC\t", offset);
		printf("v:%d t:%d ", valid, cc_type);
		printf("[%02X;%02X] ", cc_data_1, cc_data_2);

		parse_608(cc_data_1, cc_data_2);

		printf("\n");
	}
}

void parse_608(uint8_t hi, uint8_t lo) {

	if (hi >= 0x18 && hi <= 0x1f)
		hi = hi - 8;

	if (hi >= 0x20) {
		printf("<%c;", hi);

		if (lo >= 0x20)
			printf("%c>", lo);
		else
			printf(".>");
	}

	if ((hi >= 0x11 && hi <= 0x17 && lo >= 0x40 && lo <= 0x7f) ||
		(hi == 0x10 && lo >= 0x40 && lo <= 0x5f))
	{
		printf("PAC");
	}

	if ((hi >= 0x14 && hi <= 0x15 && lo >= 0x20 && lo <= 0x2f) ||
		(hi == 0x17 && lo >= 0x2e && 0x2f))
	{
		parse_608_command(hi, lo);
	}
}

void parse_608_command(uint8_t hi, uint8_t lo) {
	if (hi == 0x15) hi = 0x14;

	if (hi == 0x14 || hi == 0x1C) {
		switch (lo) {
			case 0x20: printf("RCL"); break;
			case 0x21: printf("BS "); break;
			case 0x22: printf("AOF"); break;
			case 0x23: printf("AON"); break;
			case 0x24: printf("DER"); break;
			case 0x25: printf("RU2"); break;
			case 0x26: printf("RU3"); break;
			case 0x27: printf("RU4"); break;
			case 0x28: printf("FON"); break;
			case 0x29: printf("DRC"); break;
			case 0x2A: printf("TR "); break;
			case 0x2B: printf("RTD"); break;
			case 0x2C: printf("EDM"); break;
			case 0x2D: printf("CR "); break;
			case 0x2E: printf("ENM"); break;
			case 0x2F: printf("EOC"); break;
			default: break;
		}
	}

	if (hi == 0x17 || hi == 0x1F) {
		switch (lo) {
			case 0x21: printf("TO1"); break;
			case 0x22: printf("TO2"); break;
			case 0x23: printf("TO3"); break;
			default: break;
		}
	}
}

bool read_next() {
	for (size_t i = 1; i < buffer_size; i++)
		buffer[i - 1] = buffer[i];

	buffer[buffer_size - 1] = fgetc(stdin);

	bits = 0;

	offset++;
	return feof(stdin);
}

uint32_t get_bytes(size_t n) {
	assert(n <= 4);
	assert((bits & 0x7) == 0);

	uint32_t ret = 0;
	while (n > 0) {
		size_t byte = bits >> 3;
		ret |= buffer[byte] << ((n - 1) << 3);

		bits += 8;
		n--;
	}

	return ret;
}

uint32_t get_bits(size_t n) {
	assert(n <= 32);
	
	uint32_t ret = 0;

	while (n > 0) {
		size_t byte = bits >> 3;
		size_t bit = bits & 0x7;

		uint8_t b = (buffer[byte] & (1 << (7 - bit))) ? 1 : 0;
		ret |= b << (n - 1);

		n--;
		bits++;
	}

	return ret;
}
