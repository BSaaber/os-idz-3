all: TCPEchoClient TCPEchoServer
TCPEchoClient: TCPEchoClient.c DieWithError.c
	gcc TCPEchoClient.c DieWithError.c -o server
TCPEchoServer: TCPEchoServer.c DieWithError.c
	gcc TCPEchoServer.c DieWithError.c HandleTCPClient.c -o client
clean:
	rm server client