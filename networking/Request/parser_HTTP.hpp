
#ifndef parser_HTTP_hpp
#define parser_HTTP_hpp

#include "../hdelibc_networking.hpp"
#include "../Servers/SimpleServer.hpp"
#include "../Colors.hpp"

#include <stdio.h>
#include <string>
#include <iostream>
#include <cstring>
#include <iterator>
#include <map>


namespace HDE{

    /* make an enum for GET POST and DELETE   -- 
• You need at least GET, POST, and DELETE methods.
 */


    enum method{
        GET,
        POST,
        DELETE
    };

    class parser_HTTP
    {
        private:
        static const std::string                    permisChar;


        public:


    };

}





#endif