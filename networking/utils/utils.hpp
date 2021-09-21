#ifndef utils_hpp
#define utils_hpp



#include <sstream>
#include "../utils/colors.hpp"

int ft_stoi(std::string s);

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
