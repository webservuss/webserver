NAME =		test

SRCS =		networking/Servers/test.cpp \
			networking/Servers/simple_server.cpp \
			networking/Sockets/simple_socket.cpp \
			networking/Servers/select_server.cpp \
			networking/Sockets/listen_n_bind.cpp \
			http_funct.cpp

OFILES =	$(SRCS:.cpp=.o)

FLAGS = -std=c++98
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
