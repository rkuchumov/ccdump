#ifndef H_608_H
#define H_608_H

#include <cstdint>

class cc_608
{
public:
	void parse(uint8_t hi, uint8_t lo);

private:
	void parse_command(uint8_t hi, uint8_t lo);
};

#endif /* end of include guard: 608_H */
