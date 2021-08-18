//
// Created by ruben on 14-07-21.
//
#include "../../all_libs.hpp"

// void dbe(std::string s)
// {
// 	std::cout << s << std::endl;
// }
// void db(std::string s)
// {
// 	std::cout << s;
// }

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


std::string char_string(char * buffer)
{
    std::string s;

    s = "";
    for( int k = 0;buffer[k] != '\0'; k++){
        s = s + buffer[k];
    }
    return s;
}
