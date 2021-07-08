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
        if (found < INT_MAX)e
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

//  setters
void		HDE::configServer_hpp::set_server_name(std::string srv_name)
{
    _srv_name = server_name;
}

void		HDE::configServer_hpp::set_port(int prt)
{
    _port = prt;
}

void		HDE::configServer_hpp::set_host(std::string hst)
{
    hst = _host;
}

void		HDE::configServer_hpp::set_error_page(std::string err_pg)
{
    _error_page = err_pg;
}

void		HDE::configServer_hpp::set_auto_index(int ato_ndx)
{
    _auto_index = ato_ndx;
}

void 		HDE::configServer_hpp::set_root(std::string rt)
{
    _root = rt;
}

void 		HDE::configServer_hpp::set_index(std::string ndx)
{
    _index = ndx;
}

//  getters
std::string		HDE::configServer_hpp::set_server_name()
{
    return _server_name;
}

int				HDE::configServer_hpp::set_port()
{
    return _port;
}

std::string		HDE::configServer_hpp::set_host()
{
    return _host;
}

std::string		HDE::configServer_hpp::set_error_page()
{
    return _error_page;
}

int				HDE::configServer_hpp::set_auto_index()
{
    return _auto_index;
}
std::string 	HDE::configServer_hpp::set_root()
{
    return root;
}

std::string 	HDE::configServer_hpp::set_index()
{
    return _index;
}
