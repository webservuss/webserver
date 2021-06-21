NAME =		test

SRCS =		networking/Servers/test.cpp networking/Servers/testserver.cpp networking/Servers/SimpleServer.cpp networking/Sockets/bindingSocket.cpp networking/Sockets/listeningSocket.cpp networking/Sockets/hdelibc_sockets.cpp networking/Sockets/simpleSocket.cpp networking/hdelibc_networking.cpp  hdelibc.cpp 
#  networking/Sockets/main.cpp
# networking/Sockets/connectingSocket.cpp 
OFILES =	$(SRCS:.c=.o)

FLAGS = 
# -Wall -Werror -Wextra

INCLUDES =	*.hpp

all:		$(NAME)

$(NAME):	$(OFILES) $(INCLUDES)
	g++ -o $(NAME) $(OFILES) $(FLAGS)

%.o: %.c $(INCLUDE)
	gcc $(FLAGS) -c $< -o $@

.PHONY: all clean fclean re
