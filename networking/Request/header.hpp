#ifndef header_hpp
#define header_hpp

#include <stdio.h>
#include <string>
#include <iostream>
#include <cstring>
#include <iterator>
#include <map>
  

//#include "../hdelibc_networking.hpp"
//#include "../Servers/SimpleServer.hpp"
#include "../utils/colors.hpp"


namespace HDE{

class header{

    private:
        std::string key;
        std::string value;
    public:
        header(){};
        header(const std::string& key, const std::string& value ) : key(key), value(value){}
       /* return*/         /* hhtp header   */
        void                set_value(const std::string& value);

        const std::string   get_key()const;
        std::string         serialize() const;
};

}



#endif