#ifndef utils_hpp
#define utils_hpp


#include <sstream>

int ft_stoi(std::string s);
// void db(std::string s);
// void dbe(std::string s);
std::vector<std::string> split(const std::string &s, char delim);


template <typename T>
std::string ft_numtos(T number)
{
	std::ostringstream ss;
	ss << number;
	return ss.str();
}

#endif
