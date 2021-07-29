
#ifndef parser_HTTP_hpp
#define parser_HTTP_hpp

#include "../../http.hpp"
#include "../Colors.hpp"
#include "makeMap.hpp"
#include "utils_countspace.hpp"

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


namespace HTTP{

    /* make an enum for GET POST and DELETE   -- 
• You need at least GET, POST, and DELETE methods.
 */
enum method{
    GET,
    POST,
    DELETE
};


    class re_HTTP
    {
        re_HTTP(){}
        private:
            std::string _key;
            std::string _value;
            static const std::string                   permisChar;
            std::string _headers;
            


        public:
            re_HTTP(std::string dataparser);
            void split_line(std::string line);
            std::map<std::string, std::string> mapHeader;
            
        
        //setters
        
        void		set_key(std::string key);
        void		set_value(std::string value);
        void        set_headers(std::string body);

        //getters
        std::string	 get_key();
        std:: string  request();


    };

}





#endif