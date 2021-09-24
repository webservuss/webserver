NAME =		webserver

CC =		clang++

SRCS =		networking/Servers/main.cpp \
			networking/Sockets/simple_socket.cpp \
			networking/Sockets/listen_n_bind.cpp \
			networking/utils/http_funct.cpp \
			networking/utils/utils.cpp \
			networking/Request/re_HTTP.cpp \
			networking/Servers/parser_conf.cpp \
			networking/Servers/select_server.cpp\
			networking/Respond/respond.cpp \
			networking/Respond/CGI.cpp

OFILES =	$(SRCS:.cpp=.o)

FLAGS = -std=c++98 -Wall -Werror -Wextra

all:		$(NAME)

$(NAME):	$(OFILES)
	$(CC) -o $(NAME) $(SRCS) $(FLAGS)

%.o: %.cpp
	$(CC) $(FLAGS) -c $< -o $@

clean:
	/bin/rm -f $(OFILES)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re
