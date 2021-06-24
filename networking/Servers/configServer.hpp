#ifndef configServer_hpp
#define configServer_hpp

#include <stdio.h>

#include "../hdelibc_networking.hpp"

namespace HDE{
    
class configServer_hpp{
    public:
        //complian form
        // configServer_hpp();
        configServer_hpp(std::string config_file_path);

        //setters
        void		set_server_name(std::string srv_name);
        void		set_port(int prt);
        void		set_host(std::string hst);
        void		set_error_page(std::string err_pg);
        void		set_auto_index(int ato_ndx);
        void 		set_root(std::string rt);
        void 		set_index(std::string ndx);
        //getters
        std::string		set_server_name();
        int				set_port();
        std::string		set_host();
        std::string		set_error_page();
        int				set_auto_index();
        std::string 	set_root();
        std::string 	set_index();

    private:
        std::string _server_name;
        int         _port;
        std::string _host;
        std::string _error_page;
        int         _auto_index;
        std::string _root;
        std::string _index;
        configServer_hpp();
};

}

#endif