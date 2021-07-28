#include "respond.hpp"

#include <iostream>
#include <map>
#include <time.h>
#include <sys/time.h>
#include <string>
#include <stdio.h>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

 HTTP::respond::respond(std::map < std::string, std::string > mapHeader){

    std::string findKey;
    findKey = mapHeader["GET"];
    std::string statusline = status_line(findKey);
    setDate();
    setmodified(1);
    findKey = mapHeader["Connection:"];
    setconnection(findKey);
    findKey = mapHeader["Host:"];
    setHost(findKey);
    findKey = mapHeader["Accept-Language:"];
    setLanguage(findKey);
    setbody();
     std::map<std::string, std::string>::iterator it = mapHeader.begin();
     std::cout << RED <<  "*******************    MAP REQUEST CONTAINTS   *******************\n";
     for (it=mapHeader.begin(); it!=mapHeader.end(); ++it)
         std::cout << GREEN << it->first  << BLUE << " => " << GREEN << it->second << RESET << '\n';
}

std::string HTTP::respond::status_line(std::string findKey){


  //  TODO also add a bad request if we dont find HTTP/1.1 !!!
    int n = 0;
    std::cout << findKey << std::endl;
    size_t pos = 0;
    int i = 0;
    int j = 0;
    char * needle = strdup("HTTP/1.1");
    char * c = const_cast<char*>(findKey.c_str());
    char *res = c;
    while((res = std::strstr(res, needle)) != nullptr) {
        ++res;
        j = 2;
    }
    if(j == 2)
        return("HTTP/1.1 200 OK");
    return(" ");

}


void HTTP::respond::setDate(){

        struct timeval tv;
        time_t t;
        struct tm *info;
        char buffer[64];
        gettimeofday(&tv, NULL);
        t = tv.tv_sec;
        info = localtime(&t);
        _date = strftime(buffer, sizeof buffer, "%a, %d %B %Y %H::%M::%S %Z" , info);
        _date = buffer;
        std::cout << "date :" << _date << std::endl;

}


void ::HTTP::respond::setmodified(int fileFD ){

    struct stat	stat;
    struct tm	*info;
    char		timestamp[36];

    fstat(fileFD, &stat);
    info = localtime(&stat.st_mtime);
    strftime(timestamp, 36, "%a, %d %h %Y %H:%M:%S GMT", info);
    _lastmodified.append(timestamp);
    _lastmodified.append("\r\n");
    std::cout << "last modified : " << _lastmodified <<  std::endl;


    //_response.append(last_modified);

}

void HTTP::respond::setconnection(std::string connection){

    _connection = connection;
    std::cout << "connection: " << _connection << std::endl;
}


void HTTP::respond::setHost(std::string host){

    _host = host;
    std::cout << "host: " << _host << std::endl;

}

void HTTP::respond::setLanguage(std::string contentlanguage){

    _language = contentlanguage;
    std::cout << "content language: " << _language << std::endl;

}


const std::string &HTTP::respond::getStatusline() const {

    return _statusline;
}

void HTTP::respond::setbody(){



    std::ifstream       file("Plop");
    if (file)
    {
        /*
         * Get the size of the file
         */
        file.seekg(0,std::ios::end);
        std::streampos          length = file.tellg();
        file.seekg(0,std::ios::beg);

        /*
         * Use a vector as the buffer.
         * It is exception safe and will be tidied up correctly.
         * This constructor creates a buffer of the correct length.
         * Because char is a POD data type it is not initialized.
         *
         * Then read the whole file into the buffer.
         */
        std::vector<char>       buffer(length);
        file.read(&buffer[0],length);
    }





//    std::string line;
//    int sum = 0;
//    std::ofstream file("badrequest.html");
//    std::cout << "in the body " << std::endl;
//    //const char *path = "./badrequest.html";
//    //file.open(path);
//    if(file.is_open())
//        std::cout<< RED<< "bad request is open" << RESET << std::endl;
//    else
//        std::cout << RED << " bad request is not open" << RESET << std::endl;

//    std::ifstream file("badrequest.html");
//
//    std::string content( (std::istreambuf_iterator<char>(file) ),
//                         (std::istreambuf_iterator<char>()    ) );


//    while(std::getline(file, line)){
//
//        int numofChars = line.length();
//        for(unsigned int n = 0; n< line.length(); n++)
//        {
//            if(line.at(n) == ' ' )
//                numofChars--;
//
//        }
//        sum = numofChars+sum;
//    }
//    std::cout << "num  " << sum << std::endl;
    file.close();
}


