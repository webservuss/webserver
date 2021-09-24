#include <iostream>
#include <vector>
#include <sstream>
#include <cstdlib>
#include <stdint.h>
#include "../utils/colors.hpp"

void	error_exit(const std::string &error_msg, int code)
{
	std::cerr << YELLOW <<  "error: " << error_msg << RESET <<  std::endl;
	exit(code);
}

/*
 * Splits a string by a delimiter, for instance a space (' ')
 * Returns a vector of strings which are split by the delimiter
 */

std::vector<std::string> split(const std::string &s, char delim)
{
	std::vector<std::string>	elems;
	std::stringstream			ss(s);
	std::string 				item;
	while (getline(ss, item, delim))
	{
		elems.push_back(item);
	}
	return elems;
}

int ft_stoi(std::string s)
{
	int i;
	std::istringstream(s) >> i;
	return i;
}

/**
 * hex2int
 * take a hex string and convert it to a 32bit number (max 8 hex digits)
 */
//uint32_t hex2int(char *hex) {
//	uint32_t val = 0;
//	while (*hex) {
//		/* get current character then increment */
//		uint8_t byte = *hex++;
//		/* transform hex character to the 4bit equivalent number, using the ascii table indexes */
//		if (byte >= '0' && byte <= '9') byte = byte - '0';
//		else if (byte >= 'a' && byte <='f') byte = byte - 'a' + 10;
//		else if (byte >= 'A' && byte <='F') byte = byte - 'A' + 10;
//		/* shift 4 to make space for new digit, and add the 4 bits of the new digit */
//		val = (val << 4) | (byte & 0xF);
//	}
//	return val;
//}

unsigned int hex2int(std::string &hex_str)
{
	unsigned int x;
	std::stringstream ss;
	ss << std::hex << hex_str;
	ss >> x;

	return x;
}