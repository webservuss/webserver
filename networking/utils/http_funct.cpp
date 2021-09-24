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


		/*
		 * length \r\n\
		 * [body]\r\n
		 * end 0 \r\n
		 * \r\n
		 */
int HTTP::post_expected_body(t_client_select &client, char * &buffer, int &length)
{
	std::ofstream existing_file;
	existing_file.open(client._filename.c_str(), std::ios::binary | std::ios::app);


	if (client._chunked)
	{

		std::string tmp_first_line;
		std::string buf_str(buffer);
		std::string body;
		int x = 0;
		while (!buf_str.empty())
		{

			/* get first line to get the length of chunk */
			tmp_first_line = buf_str.substr(0, buf_str.find("\r\n"));
			unsigned int unchunked_length = hex2int(tmp_first_line);
			std::cout << YELLOW << "first_line: " << tmp_first_line << " size of fline: " << tmp_first_line.size() << " unchunk_len: " << unchunked_length << RESET << std::endl;

			/* get body */
			body = buf_str.substr(buf_str.find("\r\n") + 2, unchunked_length);

			/* check if done and return 1 */
			if (unchunked_length == 0 && body.empty())
			{
//				std::cout << MAGENTA << "DONE? bfstring:\n" << buf_str << RESET << std::endl;
				existing_file.close();
				client._expect_body = false;
				client._post_done = true;
				//client._header = "HTTP/1.1 204 No Content\r\n\r\n";
				return 1;
				break;
			}

			/* write body to file */
//			if (x == 0)
				existing_file.write(body.c_str(), unchunked_length);

			/* trim body */
			std::cout << RED << buf_str.substr(buf_str.size() -20., 20) << RESET << std::endl;
			buf_str = buf_str.substr(buf_str.find("\r\n") + 2 + unchunked_length + 2); // + 2 at the end?
			std::cout << RED << buf_str.substr(0, 20) << RESET << std::endl;
			std::cout << buf_str.size() << "... length: "<< length << std::endl;

			x++;
//			if (x == 2)
//				exit(1);
		}

		std::cout << RED << "end of loop" << RESET << std::endl;
		existing_file.close();
		return 0;

//
//		if (body.size() > unchunked_length)
//		{
//
//			client._expect_body = false;
//			client._post_done = true;
//			client._header = "HTTP/1.1 204 No Content\r\n\r\n";
//			return 1;
//		}
//		return 0;
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
