all: TCPEchoClient TCPEchoServer
TCPEchoClient: TCPEchoClient.c DieWithError.c
	gcc TCPEchoClient.c DieWithError.c -o TCPEchoClient
TCPEchoServer: TCPEchoServer.c DieWithError.c
	gcc TCPEchoServer.c DieWithError.c HandleTCPClient.c -o TCPEchoServer
