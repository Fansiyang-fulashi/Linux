.PHONY:all clean
all:server client
server:TcpServer.cpp
	g++ -o $@ $^
client:TcpClient.cpp
	g++ -o $@ $^

clean:
	rm -f server client