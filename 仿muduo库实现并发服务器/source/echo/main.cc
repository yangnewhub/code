#include "echoserver.hpp"

int main()
{
    EchoServer server(8081);
    server.Start();
}