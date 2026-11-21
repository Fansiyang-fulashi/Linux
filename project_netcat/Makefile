.PHONY:all
all:server1 client1
client1:TcpClient.cpp -ljsoncpp
	g++ -o $@ $^
server1:TcpServer.cpp -ljsoncpp
	g++ -o $@ $^
.PHONY:clean
clean:
	rm -f server1 client1