#include "re_HTTP.hpp"

#include <iostream>
#include <map>
#include <iostream>
#include <string>


using namespace HTTP;

std::string methods[4] = {
        "GET",
        "POST",
        "DELETE",
};


const std::string re_HTTP::permisChar = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_.~!#$&'()*+,/:;=?@[]";

HTTP::re_HTTP::re_HTTP(std::string dataparser)
{
    std::string data;
    std::string line;
    std::string method;
    int i = 0;

    // here set the request line and define the method.
    //getMethod(); //
    std::istringstream request_data(dataparser);
    set_headers(dataparser);
    while(std::getline(request_data, data))
    {
        line = data.substr(0, data.find('\r'));

        while(i == 0) {

            std::cout << RED<< "were here? " << RESET <<std::endl;
            setRequestline(line);
            i++;
        }

        if(line.size() != 0)
            split_line(line);
        mapHeader.insert(std::pair<std::string, std::string>( _key, _value) );
    }

    std::map<std::string, std::string>::iterator it = mapHeader.begin();
    std::cout << RED <<  "*******************MAP REQUEST CONTAINTS*******************\n";
    for (it=mapHeader.begin(); it!=mapHeader.end(); ++it)
    std::cout << GREEN << it->first  << BLUE << " => " << GREEN << it->second << RESET << '\n';

}

void HTTP::re_HTTP::set_headers(std::string header) {

    std::string totalBody;
    int i = 0;
    while (header[i] != '\r')
        i++;
    std::cout << "i" << i << std::endl;
    totalBody = header.substr(i, header.size() - i);
    std::cout << "TOTAL BODY " << totalBody << std::endl;
}

void HTTP::re_HTTP::set_key(std::string key)
{
            _key = key;
           
}

void HTTP::re_HTTP::set_value(std::string value)
{
            _value = value;
         
}

void HTTP::re_HTTP::split_line(std::string line){

    set_key(line.substr(0, line.find(" ")));
    set_value(line.substr(line.find(" "), line.find('\r')));


}


std::string &re_HTTP::getMethod() {
//TODO method needs to checked if same as config otherwise (405)
    //const char* method;
    // loop through the string of methods and compare it with the set method out of the requestline.
    for (int i = 0; i < 3; i++){
        if (_method.compare(methods[i]) == 0)
            return methods[i];
    }
    if (getMethod().empty()) {
        std::cout << " ERROR: _status = 400" << std::endl;
    }
    int i = 0;
    return methods[i];
}

void re_HTTP::setMethod(const std::string &method) {
    _method = method;
}

const std::string &re_HTTP::getRequestline() const {
    return _requestline;
}

void re_HTTP::setRequestline(std::string &requestline) {

   std::cout << RED << requestline << RESET << std::endl;
   //int i;
    int found = -1;
    for(int i = 0; i < 2; i++)
    {
        // int found;
         found = requestline.find(methods[i]);
        if(found == -1)
        {
            std::cout << "NO METHOD" << std::endl; // error message.
            break;
        }
            _method = requestline.substr(found, methods[i].size());
            std::cout << RED <<"method:" << RESET << _method << std::endl;
    }
    std::cout << "here"<< std::endl;
    _requestline = requestline;
}


//get URI
const std::string &re_HTTP::getUri() const {
    return _uri;
}

//define URI
void re_HTTP::setUri(const std::string &uri) {
    _uri = uri;
}
