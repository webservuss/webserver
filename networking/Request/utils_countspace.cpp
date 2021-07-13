#include "utils_countspace.hpp"

size_t UTILS::countSpace(const std::string& str){

    size_t  i;
    for (size_t j = 0; str[j]; j++)
        if(str[j] == ' ')
            i++;
    return i;
}

