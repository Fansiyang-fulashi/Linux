.PHONY:all clean

all:server client
server:udbserver.cpp
	g++ -o $@ $^
client:udbclient.cpp
	g++ -o $@ $^

clean:
	rm -f server client