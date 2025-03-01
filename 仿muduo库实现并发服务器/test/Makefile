all:server client
server:tcp_server.cc  
	g++ -o $@ $^  -std=c++14 -lpthread -g
client:tcp_client.cc  
	g++ -o $@ $^  -std=c++14 -lpthread -g
.PHONY:clean
clean :
	rm -rf server client	