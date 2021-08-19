#include "statuscode.hpp"





void HTTP::statuscode::set_status_line()
{
    std::string total_body;
    std::cout << YELLOW << "last modified : " << _pars_server._error_page[1] << R<< std::endl;
    std::string root = _pars_server._error_page[1];
    std::ifstream file("html_pages/auto_error.html");
std:_statusline = "HTTP/1.1 ";
    if (_status_code == 404)
    {
        if ( _pars_server._error_page[0] == "404" || _pars_server._error_page[0] == "404;")
            set_no_config404(root);
        else
         _statusline.append("404 Not Found");
        _body = "<h1>403: You can't do that!</h1>\0";
    }
    else if (_status_code == 200)
        _statusline.append("200 OK");
    else if (_status_code == 403)
    {
        _statusline.append("403 Forbidden");
        _body = "<h1>403: You can't do that!</h1>\0";
    }
    else if (_status_code == 405)
    {
        _statusline.append("405 Method not Allowed");
        _body = "<h1>405: Try another method!</h1>\0";
    }
    else if (_status_code == 204)
        _statusline.append("204 No Content");
    else if (_status_code == 301)
        _statusline.append("301 Moved Permanently");
    else if(_status_code == 400)
    {
        _statusline.append("400 ");
        _body = "<h1> 400 :Bad Request The request could not be understood by the server due to malformed syntax. The client SHOULD NOT repeat the request without modifications. </h1> \0";
    }
    else if(_status_code == 413)
    {
        _statusline.append("113 ");
        _body = "<h1> 400 : Request Entity Too Large \0";
    }
    else if (file.is_open())
        {
            std::cout << " THERE IS A CONFIG " << std::endl;
            total_body = std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
            _body = total_body;
        }
    file.close();
    setcontenttype("text/html");
    setContentlen(_body);
}
