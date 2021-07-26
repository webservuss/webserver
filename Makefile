NAME =		test

SRCS =		networking/Servers/test.cpp \
			networking/Sockets/simple_socket.cpp \
			networking/Sockets/listen_n_bind.cpp \
			networking/utils/http_funct.cpp \
			networking/utils/utils.cpp \
			networking/Request/re_HTTP.cpp \
			networking/Request/utils_HTTP.cpp \
			networking/Servers/parser_config.cpp \
			networking/Servers/parser_conf.cpp \
			networking/Request/split.cpp  \
			networking/Request/request.cpp \
			networking/Servers/select_server.cpp 

OFILES =	$(SRCS:.cpp=.o)

FLAGS = -std=c++98 -g -fsanitize=address
# -Wall -Werror -Wextra

INCLUDES =	*.hpp

all:		$(NAME)

$(NAME):	$(OFILES) $(INCLUDES)
	g++ -o $(NAME) $(SRCS) $(FLAGS)

%.o: %.cpp $(INCLUDE)
	gcc $(FLAGS) -c $< -o $@

clean:
	/bin/rm -f $(OFILES)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re
