
CC=clang++
CFLAGS= -Wall -Wextra -Werror -std=c++11 -g -Iinclude/
LDFLAGS=
EXEC=server
VPATH = src
SRC=main.cpp SocketServeur.class.cpp Utility.cpp SocketClient.class.cpp\
	Socket.class.cpp SocketManager.class.cpp IRCMessage.class.cpp\
	MessageMediator.class.cpp Client.class.cpp User.class.cpp\
	IRCServer.class.cpp ClientManager.class.cpp ReplyManager.class.cpp\
	Service.class.cpp ChannelManager.class.cpp Channel.class.cpp \
	ServerClient.class.cpp Observer.class.cpp RoutingTable.class.cpp
OBJ=$(addprefix obj_irc/, $(SRC:.cpp=.o))

all: $(EXEC)

$(EXEC): $(OBJ)
	@$(CC) -o $@ $^ $(LDFLAGS)

obj_irc/%.o: %.cpp
	@mkdir obj_irc 2> /dev/null || true
	@$(CC) $(CFLAGS) -o $@ -c $< 

.PHONY: clean fclean

clean:
	@rm -rf obj_irc
	@rmdir obj_irc 2> /dev/null || true

fclean: clean
	@rm -rf $(EXEC)

re: fclean all