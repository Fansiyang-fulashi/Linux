.PHONY:all
all:main main2
main:main.cpp
	g++ -o $@ $^ -std=c++11 -lpthread -g
main2:main2.cpp
	g++ -o $@ $^ -std=c++11 -lpthread -g

.PHONY:clean
clean:
	rm -f main mian2