all:server client
server:main.cc
	g++ -o $@ $^
client:client.cc
	g++ -o $@ $^
.PHONY:clean
clean :
	rm -rf server client