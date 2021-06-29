
#ifndef parser_HTTP_hpp
#define parser_HTTP_hpp

#include "../hdelibc_networking.hpp"
#include "../Servers/SimpleServer.hpp"
#include "split.hpp"
#include "../Colors.hpp"
#include "makeMap.hpp"

#include <stdio.h>
#include <string>
#include <iostream>
#include <cstring>
#include <iterator>
#include <sstream>
#include <sstream>
#include <vector>
#include <iostream>
#include <map>
#include <string>
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
        parser_HTTP(){}
        private:
        static const std::string                    permisChar;


        public:
        static const std::map<std::string, method>  mapMethod;
        parser_HTTP(std::string dataparser);

    };

}





#endif