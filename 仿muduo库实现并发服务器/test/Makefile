all:server client
server:tcp_server.cc 
	g++ -o $@ $^ -g
client:tcp_client.cc
	g++ -o $@ $^ -g
.PHONY:clean
clean :
	rm -rf server client