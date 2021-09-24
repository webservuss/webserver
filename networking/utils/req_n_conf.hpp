#ifndef req_n_conf_hpp
# define req_n_conf_hpp

#include <map>
#include "../Servers/parser_conf.hpp"

namespace HTTP {

    /* we receive here the map with all the request details and we receive here a struct withe server parser */
    typedef struct								s_req_n_config {
        std::map <std::string, std::string>    _req_map;
        t_server                               _parser_server;
    }   										t_req_n_config;
}

#endif