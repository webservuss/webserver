NAME =		test

SRCS =		networking/Servers/test.cpp networking/Servers/testserver.cpp \
networking/Servers/SimpleServer.cpp networking/Sockets/bindingSocket.cpp \
networking/Sockets/listeningSocket.cpp networking/Sockets/hdelibc_sockets.cpp \
networking/Sockets/simpleSocket.cpp networking/hdelibc_networking.cpp  hdelibc.cpp\
networking/Request/request.cpp \
networking/Request/parser_HTTP.cpp networking/Request/utils_HTTP.cpp \
networking/Request/split.cpp



OFILES =	$(SRCS:.c=.o)

FLAGS = 
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
