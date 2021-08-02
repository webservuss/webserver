#ifndef req_n_conf_hpp
# define req_n_conf_hpp

#include "../../all_libs.hpp"
#include "../Servers/parser_conf.hpp"
namespace HTTP {

    typedef struct s_req_n_config {
        std::map <std::string, std::string>    _req_map;
        t_server                               _parser_server;
    }   t_req_n_config;
}

#endif