#ifndef parser_config_hpp
#define parser_config_hpp

#include <stdio.h>

#include "../hdelibc_networking.hpp"
#include "../hdelibc_networking.hpp"
#include "../Servers/SimpleServer.hpp"
#include "split.hpp"
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

namespace HDE{

class parser_config
{

    parser_config(){}
        private:
            std::string _key;
            std::string _value;
            static const std::string                    permisChar;

        public:
            parser_config(std::string dataparser);
            void split_line(std::string line);
            static const std::map<std::string, method>  mapMethod;
            std::map<std::string, std::string> mapHeader;
         
        void		set_server_name(std::string srv_name);
        void		set_port(int prt);
        void		set_host(std::string hst);
        void		set_error_page(std::string err_pg);
        void		set_auto_index(int ato_ndx);
        void 		set_root(std::string rt);
        void 		set_index(std::string ndx);

};

}

#endif