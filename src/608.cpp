#include <cstdio>
#include <cstdint>
#include <cassert>

#include "608.h"

void cc_608::parse(uint8_t hi, uint8_t lo) {

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
		parse_command(hi, lo);
	}
}

void cc_608::parse_command(uint8_t hi, uint8_t lo) {
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
