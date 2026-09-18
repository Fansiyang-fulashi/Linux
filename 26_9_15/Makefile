SER=server.cpp
CLI=client.cpp
SEREXE=server
CLIEXE=client

.PHONY:all clean
all:$(SEREXE) $(CLIEXE)
$(SEREXE):$(SER)
	g++ -o $@ $^
	
$(CLIEXE):$(CLI)
	g++ -o $@ $^

clean:
	rm -f $(CLIEXE) $(SEREXE)