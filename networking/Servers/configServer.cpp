#include "SimpleServer.hpp"

HDE::configServer_hpp::configServer_hpp(ifstream configfileStream)
{
    
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
    return _root;
}

std::string 	HDE::configServer_hpp::set_index()
{
    return _index;
}
