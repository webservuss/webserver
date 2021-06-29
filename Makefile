NAME =		test

SRCS =		networking/Servers/test.cpp \
			networking/Servers/simple_server.cpp \
			networking/Sockets/simple_socket.cpp \
			networking/Servers/select_server.cpp \
			networking/Sockets/listen_n_bind.cpp


OFILES =	$(SRCS:.c=.o)

FLAGS = -std=c++98
# -Wall -Werror -Wextra

INCLUDES =	*.hpp

all:		$(NAME)

$(NAME):	$(OFILES) $(INCLUDES)
	g++ -o $(NAME) $(SRCS) $(FLAGS)

%.o: %.c $(INCLUDE)
	gcc $(FLAGS) -c $< -o $@

clean:
	rm -rf $(NAME)

.PHONY: all clean fclean re
