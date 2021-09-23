#include <stdlib.h>
#include <fcntl.h>
#include <fstream>
#include "http_funct.hpp"

/* set non-blocking: to set a specific flag and leave the other flags as-is,
then you must F_GETFL the old flags, | the new flag in, and then F_SETFL the result
as two separate system calls; */
void                HTTP::set_non_blocking(int sock)
{
//    int opts;
	if (fcntl(sock, F_SETFL, O_NONBLOCK) < 0)
		exit(1);
	else
		return;
}

int HTTP::post_expected_body(t_client_select &client, char * &buffer, int &length)
{

	std::ofstream existing_file;
	existing_file.open(client._filename.c_str(), std::ios::binary | std::ios::app);


	std::cout << RED << __FILE__ << " " << __LINE__ << "done?" << std::endl;
	if (client._chunked)
	{
		std::cout << RED << __FILE__ << " " << __LINE__ << "done?" << std::endl;
		std::string tmp_first_line(buffer);
		/* get rid of header and empty space (\r\n\r\n) */
		tmp_first_line = tmp_first_line.substr(tmp_first_line.find("\r\n\r\n") + 4);
		/* we now can set this as the body. The closing 0 (end of chunked-request) is
		 * included but we can ignore this since we know the length to write */
		std::string tmp_body = tmp_first_line.substr(tmp_first_line.find("\r\n") + 2);
		/* trim the first line to be able to read the bytes we need to write */
		tmp_first_line = tmp_first_line.substr(0, tmp_first_line.find("\r\n"));
		int unchunked_length = hex2int((char *)tmp_first_line.c_str());

		/* write to file and return */
		existing_file.write(tmp_body.c_str(), unchunked_length);
		existing_file.close();
		client._total_body_length += unchunked_length;

		std::cout << __FILE__ << "tmp b s: " << tmp_body.size() << ". unch length: " << unchunked_length << std::endl;
		if ((int)tmp_body.size() > unchunked_length)
		{
			std::cout << RED << __FILE__ << " " << __LINE__ << "done?" << std::endl;
			client._expect_body = false;
			client._post_done = true;
//			client._header = "HTTP/1.1 204 No Content\r\n\r\n";
			return 1;
		}
		return 0;
	}
	else
	{
		existing_file.write(&buffer[0], length);
		client._total_body_length += length;

		if (length == 0 || (client._total_body_length == client._content_length)) {
			client._expect_body = false;
			client._post_done = true;
			return 1;
		}
		return 0;

	}



}
