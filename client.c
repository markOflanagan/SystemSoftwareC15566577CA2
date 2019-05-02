#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
void sendIt(int SID);

int main(int argc , char *argv[])
{
	//client variables
	
	int SID;
	struct sockaddr_in server;
	char cMessage[500];
	char sMessage[500];


	//socket creation 

	SID = socket(AF_INET,SOCK_STREAM, 0);

	if(SID == -1)
	{
		printf("socket was not created \n");
	}
	else
	{
		printf("socket created \n");
	}	
	
	//SOCEKT VARIABLES 
	//
	//port connection
	server.sin_port = htons(8801);
	//server address
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	//ipv4 protocol
	server.sin_family = AF_INET;



	//connecting to a server
	//

	if(connect(SID,(struct sockaddr *)&server,sizeof(server))<0)
	{
		printf("connection has failed \n");
		return 1;
	}


	printf("connection succesful \n");


	while(1)
	{
		printf("\n Enter Message: ");
		scanf("%s",cMessage);

		//sending data
		//

		if(send(SID,cMessage,strlen(cMessage),0)<0)
		{
			printf("Sneding Failed \n");
			return 1;
		}

		//Server Reply
		//

		if(recv(SID,sMessage,500,0)<0)
		{
			printf("IO Error \n");
			break;
		}	

		printf("\nServer sent: ");
		printf(sMessage);
	
	}	

	close(SID);
	return 0;


}

void sendIt(int SID)
{

	//logic to handle to sending of file from client side to server side

	char file_buffer[512];

	char* file_name  =  "/home/markoflanagan/CA2/clientfiles/client.txt";

	printf("[Client] Sending %s to the Server... ", file_name);
	FILE *file_open = fopen(file_name, "r");
	bzero(file_buffer, 512); 
	int block_size,i=0;
	while((block_size = fread(file_buffer, sizeof(char), 512, file_open)) > 0)
	{
		printf("Data Sent %d = %d\n",i,block_size);
		if(send(SID, file_buffer, block_size, 0) < 0) {
		    exit(1);
		}
		bzero(file_buffer, 512);
		i++;
	}

	fclose(file_open);
}
