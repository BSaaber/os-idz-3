all: client server skameika
client: TCPEchoClient.c DieWithError.c
	gcc TCPEchoClient.c DieWithError.c -o client
server: TCPEchoServer.c DieWithError.c
	gcc TCPEchoServer.c DieWithError.c HandleTCPClient.c -o server
skameika: TCPEchoSkameika.c DieWithError.c
	gcc TCPEchoSkameika.c DieWithError.c HandleSkameikaTCPClient.c -o skameika

clean:
	rm server client skameika