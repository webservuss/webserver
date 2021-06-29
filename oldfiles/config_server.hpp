#ifndef config_server_hpp
#define config_server_hpp

#include <stdio.h>
#include <fstream>
#include <string>
#include <iostream>
#include <vector>

#include "../../http.hpp"
#include "simple_server.hpp"

namespace HTTP {
    
class config_server {
    public:
        //complian form
        config_server() {}
        config_server(std::ifstream & configfileStream);
        config_server (const config_server& x);
        config_server& operator=(const config_server& x);

        //setters
        void		set_server_name(std::string srv_name);
        void		set_port(int prt);
        void		set_host(std::string hst);
        void		set_error_page(std::string err_pg);
        void		set_auto_index(int ato_ndx);
        void 		set_root(std::string rt);
        void 		set_index(std::string ndx);
        //getters
        std::string		get_server_name();
        int				get_port();
        std::string		get_host();
        std::string		get_error_page();
        int				get_auto_index();
        std::string 	get_root();
        std::string 	get_index();

    private:
        std::string _server_name;
        int         _port;
        std::string _host;
        std::string _error_page;
        int         _auto_index;
        std::string _root;
        std::string _index;
        // config_server();
};

}

#endif
