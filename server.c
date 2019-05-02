#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

void *server_handler (void *fd_pointer);


//global varaibles
char Users[50];
char Folders[50];

int main(int argc, char *argv[])
{


	//socket descriptor
	int sDescriptor;
	// client descriptor
	int cDescriptor;
	//connection size
	int ConnSize;
	//socket size
	int READSIZE;
	//socket address 	
	struct sockaddr_in server , client;
	//thread socket variable
	int *thread_sock;	

	//thread variables 

	pthread_mutex_t logLock;
	pthread_mutex_t fileLock;
	pthread_mutex_t authLock;

	//socket creation 
	//AF_INET used to designate addresses for the socket server 
	//SOCK_STREAM used for connecttion based protocol either TCP or UDP 

	sDescriptor = socket(AF_INET, SOCK_STREAM, 0);

	if(sDescriptor == -1)
	{
		printf("Socket was not created \n");
	}
	else
	{
		printf("Socket was created \n");
	}
	//initialising threads
	pthread_mutex_init(&logLock,NULL);
	pthread_mutex_init(&fileLock,NULL);
	pthread_mutex_init(&authLock,NULL);

	//initalising sockets 
	//assign a port value to the sever	
	server.sin_port = htons( 8801 );
	//assign the sruture of IPV4 to the server 
	server.sin_family = AF_INET;
	//allowing the server to accept any connection from any internet address
	server.sin_addr.s_addr = INADDR_ANY;



	//Socekt binding
	if(bind(sDescriptor,(struct socketaddr *)&server, sizeof(server))<0)
	{
		perror("Binding failed to complete \n");
		return 1;
	}
	else 	
	{
		printf("Bind has conpleted \n");
	}	

	//waiting for conenction
	listen(sDescriptor,5);

	//allow connection occur 
	printf("Please wait for incoming connection \n");

	ConnSize = sizeof(struct sockaddr_in);

	//accept connection 
	cDescriptor = accept(sDescriptor, (struct socketaddr *)&client, (socklen_t*)&ConnSize);

	if(cDescriptor < 0)
	{
		perror("Can't esatblish connection");
		return 1;
	}
	else
	{
		printf("Connection established No. %d \n",cDescriptor);

		//Client thread  vairables 
		pthread_t cThread;
		//new socket for the newly created thread
		thread_sock = malloc(1);
		*thread_sock = cDescriptor;
		//creating the thread 
		pthread_create(&cThread,NULL,server_handler,(void*)thread_sock);
	}
}



//logic of all server side actions 
void *server_handler (void *fd_pointer)
{
	printf("Server Handler! User %d \n",pthread_self());
	int sock = *(int *)fd_pointer;
	//read & writing variables
	int r_size , w_size;
	char *message;
	//message that the client sends over 
	static char client_message[500];
	message = "\n Hello Welcome To The Server \n";

	static int send_once = 0;

	if(send_once < 0)
	{
		send_once++;
	}
	while((r_size = recv(sock,client_message,500,0)) >0)
	{
		printf("Read Size %d \n",r_size);
		write(sock,client_message,strlen(client_message));
	}
	if(r_size == 0 )
	{
		puts("Clients Disconnected");
		memset(client_message,'/0/',500);
		fflush(stdout);
	}
	else if(r_size == -1)
	{
		perror("recv failing");
	}

	//logic for the reciving of the  clients message
	//buffer that is recieved 512 is max for transfer at one time 
	char file_buffer[512];
	char* file_name = "/home/markoflanagan/CA2/serverfiles/server.txt";

	FILE *file_open = fopen(file_name, "w");

	if(file_open == NULL)
	{
	 	printf("file %s Cannot be opened on server",file_name);
	}
	else
	{
	      bzero(file_buffer, 512); 
              int block_size = 0;
              int i=0;
              while((block_size = recv(sock, file_buffer, 512, 0)) > 0) 
         	{
                 printf("Data Received %d = %d\n",i,block_size);
                 int write_sz = fwrite(file_buffer, sizeof(char), block_size, file_open);
                 bzero(file_buffer, 512);
                 i++;
	 }
           printf("Transfer Complete \n");
	  fclose(file_open);
	}
	close(fd_pointer);

	return 0;
}

