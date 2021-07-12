/*A namespace is a declarative region that provides a 
scope to the identifiers (the names of types, functions, 
variables, etc) inside it. Namespaces are used to organize code into 
logical groups and to prevent name collisions that can occur
 especially when your code base includes multiple libraries. */


#ifndef request_hpp
#define request_hpp

#include <stdio.h>
#include <string>
#include <iostream>
#include <cstring>
#include <iterator>
#include <map>
  

#include "../../http.hpp"
#include "../Servers/simple_server.hpp"
#include "../Colors.hpp"

namespace HDE{

/* make an enum for GET DELETE CONNECT */



/* method like get etc  */
class request {


    public:
    /* receive  a string with httml details from buffer*/
    request(){}
    /* this we need to get from buffer.*/
    request(std::string myrequest);
    //std::string myrequest;
    /*set*/
    void		set_host(std::string localhost);		
    void        set_connection(std::string connection);
    void		set_useragent(std::string useragent); 
    void        set_accept(std::string accept);

    /*get*/

    private:

    // Version version;
    // std::string _method;
    // std::string _resource;
    // std::map<std::string, Header> headers;
    // std::string myrequest;
    // std::string _Host;
    // std::string _Connection;
    // std::string _UserAgent;
    // std::string _Accept;


};


}
    

#endif