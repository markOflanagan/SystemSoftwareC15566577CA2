mServer : server.o
	gcc -o mServer server.o -lpthread

mClient : client.o
	gcc -o mClient client.o

server.o : server.c
	gcc -c server.c -lpthread

client.o : client.c
	gcc -c client.c

clean :
	rm myServer myClient server.o client.o
