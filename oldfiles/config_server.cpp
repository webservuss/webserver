#include "config_server.hpp"
#include <unistd.h>
#include <stdio.h>

#include "simple_server.hpp"

HTTP::config_server::config_server(std::ifstream &configfileStream)
{
    std::string line;
    // std::vector<std::string> config_variables = {"server_name", "listen", "host", "error_page", "autoindex", "root", "index"};

    while (std::getline(configfileStream, line))
    {
        std::cout << "line before trim:   [" << line  << "]" << std::endl;
        std::size_t found = line.find("root");
        if (found < INT_MAX)
        {
            while (line[found] != ' ')
                found++;
            std::string nxt;
            int sizey = line.length();
            std::cout << "sizey is" << sizey << " found is " << found << std::endl;
            nxt = line.substr(found, sizey - found);
            std::cout << "last bit is [" << nxt << "]" << std::endl;
            // return ;
        }
    }
    // // close file
    return ;
}


HTTP::config_server& HTTP::config_server::operator=(const HTTP::config_server& x)
{
    _server_name = x._server_name;
    _port = x._port;
    _host = x._host;
    _error_page = x._error_page;
    _auto_index = x._auto_index;
    _root = x._root;
    _index = x._index;
    return *this;
}

HTTP::config_server::config_server(const HTTP::config_server& x)
{
    _server_name = x._server_name;
    _port = x._port;
    _host = x._host;
    _error_page = x._error_page;
    _auto_index = x._auto_index;
    _root = x._root;
    _index = x._index;
}


//  setters
void		HTTP::config_server::set_server_name(std::string srv_name)
{
    _server_name = srv_name;
}

void		HTTP::config_server::set_port(int prt)
{
    _port = prt;
}

void		HTTP::config_server::set_host(std::string hst)
{
    _host = hst;
}

void		HTTP::config_server::set_error_page(std::string err_pg)
{
    _error_page = err_pg;
}

void		HTTP::config_server::set_auto_index(int ato_ndx)
{
    _auto_index = ato_ndx;
}

void 		HTTP::config_server::set_root(std::string rt)
{
    _root = rt;
}

void 		HTTP::config_server::set_index(std::string ndx)
{
    _index = ndx;
}

//  getters
std::string		HTTP::config_server::get_server_name()
{
    return _server_name;
}

int				HTTP::config_server::get_port()
{
    return _port;
}

std::string		HTTP::config_server::get_host()
{
    return _host;
}

std::string		HTTP::config_server::get_error_page()
{
    return _error_page;
}

int				HTTP::config_server::get_auto_index()
{
    return _auto_index;
}
std::string 	HTTP::config_server::get_root()
{
    return _root;
}

std::string 	HTTP::config_server::get_index()
{
    return _index;
}
