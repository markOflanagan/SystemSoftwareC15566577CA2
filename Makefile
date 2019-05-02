myServer : server.o
	gcc -o myServer server.o -lpthread

myClient : client.o
	gcc -o myClient client.o
 
server.o : server.c
	gcc -c server.c -lpthread

client.o : client.c
	gcc -c client.c

clean :
	rm myServer myClient server.o client.o 
