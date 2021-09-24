#include <stdlib.h>
#include <fcntl.h>
#include <fstream>
#include <cstring>
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



static std::string get_first_line_of_vec(std::vector<char> buf_vec)
{
	std::string first_line;


	for (int i = 0; buf_vec[i] != '\r' && buf_vec[i] != '\n'; i++)
	{
		first_line.push_back(buf_vec[i]);
	}
	return first_line;
}

static char *get_body_from_vec(std::vector<char> buf_vec, unsigned int chunk_length, int start)
{
	char * body = new char [chunk_length + 1];
	for (unsigned int i = 0; i < chunk_length; i++)
		body[i] = buf_vec[start + i];
	body[chunk_length] = '\0';
	return body;

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
		char *body;

		std::vector<char> buf_vec;
		buf_vec.reserve(length);
		for (int i = 0; i < length; i++)
			buf_vec.push_back(buffer[i]);

		std::cout << buf_vec.size() << " l: " << length << std::endl;

		while (!buf_str.empty())
		{

			/* get first line to get the length of chunk */
			if (buf_vec.size() == 0)
				return 0;
			tmp_first_line = get_first_line_of_vec(buf_vec);

			unsigned int unchunked_length = hex2int(tmp_first_line);

			/* get body */
			body = get_body_from_vec(buf_vec, unchunked_length, tmp_first_line.size() + 2);

			/* check if done and return 1 */
			if (unchunked_length == 0 && body && body[0] != '0')
			{
				existing_file.close();
				client._expect_body = false;
				client._post_done = true;
				client._chunked = false;
				return 1;
				break;
			}

			/* write body to file */
			existing_file.write(body, unchunked_length);

			/* trim body */
			buf_vec.erase(buf_vec.begin(), buf_vec.begin() + tmp_first_line.size() + 2 + unchunked_length + 2);

		}
		existing_file.close();
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
