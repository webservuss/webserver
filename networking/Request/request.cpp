#include "request.hpp"
#include <sstream>
#include <vector>
#include <iostream>




/* loop through the string  */
HDE::request::request(std::string myrequest)
{
    std::string str;
    std::istringstream request_str(myrequest);

    

    std::cout << "here"<<std::endl;
    std::cout <<" string "<< myrequest<< std::endl;
    while(std::getline(request_str, str))
    {
         std::cout <<" line is: "<< str<< std::endl;
    }
    /* setting your vecotr to string */
    std::vector<std::string> requestVec;

    requestVec.push_back(str);
    std::cout << "VECTOR" << std::endl;
   // std::cout << requestVec.size << std::endl;

    for(unsigned int i = 0; i < requestVec.size(); i++){
        std::cout << requestVec[i] << " " << std::endl;

    }
    

}

