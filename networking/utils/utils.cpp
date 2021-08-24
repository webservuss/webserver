#include <iostream>
#include <vector>
#include <sstream>
// https://stackoverflow.com/questions/236129/how-do-i-iterate-over-the-words-of-a-string/39359311
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
