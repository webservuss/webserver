#include "header.hpp"
#include <sstream>
#include <vector>
#include <iostream>


//HDE::header::header(const std::string& key, const std::string& value ) : key(key), value(value)
//{}

void HDE::header::set_value(const std::string& value)
{
    this->value = value;
}

const std::string HDE::header::get_key()const {

    return this->key;

}

std::string HDE::header::serialize()const {

    std::string header;
    header += this->key;
    header += ": ";
    
}