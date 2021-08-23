#include "re_HTTP.hpp"
#include <sstream>

std::string methods[3] = {
        "GET",
        "POST",
        "DELETE",
};

HTTP::re_HTTP::re_HTTP(std::string dataparser)
{
    int j;
    int len;
    int i;
    std::string data;
    std::string key;
    std::string value;
    std::string line;
    std::string method;
    std::istringstream request_data(dataparser);

    j = 0;
    i = 0;
    len = _totalBody.length();
    set_headers(dataparser);
    while(std::getline(request_data, data ) && !request_data.eof())
    {
        line = data.substr(0, data.find('\r'));
        if (i == 0)
            i = set_request_line(line);
        if(line.size() != 0)
        {
            key = (line.substr(0, line.find(" ")));
            value = (line.substr(line.find(" "), line.find('\r')));
            _map_header.insert(std::pair<std::string, std::string>(key, value));
        }
        j++;
    }
}

// CAN SOMEONE CHECK COPY CONSTRUCTOR AND ASSIGNMENT OPPERATOR
/*copy constructor */
HTTP::re_HTTP::re_HTTP(const re_HTTP& x)
{ // TEST THIS // change
    _headers = x._headers;
    _method = x._method;
    _uri = x._uri;
    _totalBody = x._totalBody;
    _protocol = x._protocol;
}

/*assignment operator */
HTTP::re_HTTP& HTTP::re_HTTP::operator=(const re_HTTP& x)
{ // TEST THIS
    _headers = x._headers;
    _method = x._method;
    _uri = x._uri;
    _totalBody = x._totalBody;
    _protocol = x._protocol;
    return *this;
}


void HTTP::re_HTTP::set_headers(std::string header) {

    int i;

    i = 0;
    while (header[i] != '\r')
        i++;
    _totalBody = header.substr(i, header.size() - i);
    _method = "";
    _uri = "";
    _protocol = "";
}

int HTTP::re_HTTP::set_request_line(std::string &requestline) 
{
    int found;
    
    found = -1;
    for(int i = 0; i < 3; i++)
    {
        found = requestline.find(methods[i]);
        if(found != -1)
        {
            _method = requestline.substr(found, methods[i].size());
            std::string tmp = requestline.substr(_method.size() + 1, requestline.size()- methods[i].size());
            _uri = tmp.substr(0, tmp.find(' '));
            _uri = _uri.substr(1, _uri.size() - 1);
            _protocol = tmp.substr(_uri.size() + 2, tmp.size() - _uri.size());
            break;
        }
    }
    _map_header.insert(std::pair<std::string, std::string>( "METHOD", _method));
    _map_header.insert(std::pair<std::string, std::string>( "URI", _uri) );
    _map_header.insert(std::pair<std::string, std::string>( "PROTOCOL", _protocol));
    return (1);
}

HTTP::re_HTTP::~re_HTTP()
{
    // std::cout << "destructor" << std::endl;
    // for (std::map<std::string, std::string>::iterator itft = _map_header.begin(); itft!=_map_header.end(); ++itft)
    //     std::cout << itft->first << " => " << itft->second << '\n';
}
