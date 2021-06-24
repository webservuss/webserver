#include "configServer.hpp"

HDE::configServer::configServer(std::ifstream configfileStream)
{
    std::string line;
    
    if (!configfileStream.is_open())
        return;
    while (std::getline(configfileStream, line))
    {
        std::cout << "line is: " << std::endl;
    }
    //close file
    return ;
}

HDE::configServer& HDE::configServer::operator=(const HDE::configServer& x)
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

HDE::configServer::configServer(const HDE::configServer& x)
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
void		HDE::configServer::set_server_name(std::string srv_name)
{
    _server_name = srv_name;
}

void		HDE::configServer::set_port(int prt)
{
    _port = prt;
}

void		HDE::configServer::set_host(std::string hst)
{
    _host = hst;
}

void		HDE::configServer::set_error_page(std::string err_pg)
{
    _error_page = err_pg;
}

void		HDE::configServer::set_auto_index(int ato_ndx)
{
    _auto_index = ato_ndx;
}

void 		HDE::configServer::set_root(std::string rt)
{
    _root = rt;
}

void 		HDE::configServer::set_index(std::string ndx)
{
    _index = ndx;
}

//  getters
std::string		HDE::configServer::get_server_name()
{
    return _server_name;
}

int				HDE::configServer::get_port()
{
    return _port;
}

std::string		HDE::configServer::get_host()
{
    return _host;
}

std::string		HDE::configServer::get_error_page()
{
    return _error_page;
}

int				HDE::configServer::get_auto_index()
{
    return _auto_index;
}
std::string 	HDE::configServer::get_root()
{
    return _root;
}

std::string 	HDE::configServer::get_index()
{
    return _index;
}
