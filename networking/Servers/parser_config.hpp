#ifndef parser_config_hpp
#define parser_config_hpp

#include "../../http.hpp"
// #include "./simple_server.hpp"
#include "../utils/colors.hpp"
//#include "split.hpp"
//#include "makeMap.hpp"
//#include "utils_countspace.hpp"

namespace HDE{

class parser_config
{

        enum namec{
        GET,
        POST,
        DELETE
    };

    parser_config(){}
        private:
            static const std::string                    permisChar;
            std::string _server_name;
            int         _port;
            std::string _host;
            std::string _error_page;
            int         _auto_index;
            std::string _root;
            std::string _index;
            std::string _key;
            std::string _value;

        public:
            parser_config(std::ifstream& file);
            void parser_config_open();
            void split_config(std::string line);
            void split_line(std::string line);
            static const std::map<std::string, namec>  mapMethod;
            std::map<std::string, std::string> mapconfig;
         
        void		set_server_name(std::string server);
        void		set_port(int prt);
        void		set_host(std::string hst);
        void		set_error_page(std::string err_pg);
        void		set_auto_index(int ato_ndx);
        void 		set_root(std::string rt);
        void 		set_index(std::string ndx);
        void        set_key(std::string key);
        void        set_value(std::string value);


};

}

#endif