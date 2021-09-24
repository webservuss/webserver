#ifndef utils_hpp
#define utils_hpp

#include <sstream>
#include <stdint.h>

#include "../utils/colors.hpp"

int ft_stoi(std::string s);
//uint32_t hex2int(char *hex);
unsigned int hex2int(std::string &hex_str);

std::vector<std::string> split(const std::string &s, char delim);

void	error_exit(const std::string &errMsg, int code);
template <typename T>
std::string ft_numtos(T number)
{
	std::ostringstream ss;
	ss << number;
	return ss.str();
}

#endif
