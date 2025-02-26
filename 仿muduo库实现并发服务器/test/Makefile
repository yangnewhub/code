all:server client
server:tcp_server.cc  
	g++ -o $@ $^  -std=c++14 -lpthread
client:tcp_client.cc  
	g++ -o $@ $^  -std=c++14 -lpthread
.PHONY:clean
clean :
	rm -rf server client