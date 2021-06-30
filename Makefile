NAME =		test

SRCS =		networking/servers/test.cpp \
			networking/servers/simple_server.cpp \
			networking/sockets/simple_socket.cpp \
			networking/servers/select_server.cpp \
			networking/sockets/listen_n_bind.cpp \
			http_funct.cpp

OFILES =	$(SRCS:.c=.o)

FLAGS = -std=c++98
# -Wall -Werror -Wextra

INCLUDES =	*.hpp

all:		$(NAME)

$(NAME):	$(OFILES) $(INCLUDES)
	g++ -o $(NAME) $(SRCS) $(FLAGS)

%.o: %.cpp $(INCLUDE)
	gcc $(FLAGS) -c $< -o $@

clean:

	rm -rf $(NAME)

fclean: clean

re: fclean all

.PHONY: all clean fclean re
