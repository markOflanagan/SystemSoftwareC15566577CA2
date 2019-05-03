#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

void groupIDHandler(char *user);
void *server_handler (void *fd_pointer);
void userIDHandler(int uID);
//void groupIDHandler();
//global varaibles
char Users[50];
char Folders[50];
int uID=0;
gid_t gIDS[20];

int main(int argc, char *argv[])
{


	//socket descriptor
	int sDescriptor;
	// client descriptor
	int cDescriptor;
	//connection size
	int ConnSize;
	socklen_t clien;
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
	server.sin_port = htons(8801);
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
		printf("Bind has completed \n");
	}

	//waiting for conenction
	listen(sDescriptor,50);

	//allow connection occur 
	printf("Please wait for incoming connection \n");

	clien = sizeof(client);
	//accept connection 
	while ((cDescriptor = accept(sDescriptor,(struct socketaddr *)&client,&clien)))
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
	//socket variable to be used in below while loop 
	int sock = *(int *)fd_pointer;
	//read & writing variables
	int r_size , w_size;
	//char *message;
	//message that the client sends over 
	static char client_message[500];
	//message = "\n Hello Welcome To The Server \n";

	while(1)
	{
		r_size = recv(sock,client_message,500,0);
		//if the read size is equal to 0 aka nothin is present the client is disconnected 
		if(r_size == 0 )
		{
			puts("\n Clients Disconnected \n");
			memset(client_message,'\0',500);
			fflush(stdout);
			break;
		}
		//

		else
		{
			//printf("hello from inside the else");
			printf("\n %s \n",client_message);
		//logic for the reciving of the  clients message
		//buffer that is recieved 512 is max for transfer at one time 
		
			char file_buffer[512];
			char* file_name = "/home/markoflanagan/CA2/serverfiles/server.txt";

			FILE *file_open = fopen(file_name, "w");

			if(file_open == NULL)
			{
	 			printf("file %s Cannot be opened on server \n",file_name);
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
		//once the  transfer has been complete 
           	 		printf("Transfer Complete \n");
	  	 		fclose(file_open);
			}
		
		}
	}
	close(fd_pointer);

	return 0;
}

/*

//handles group ids ;ogic
void groupIDHandler(char *user)
{
//varaibles needed fro group fuinctionality
	int j, ngroups;
	gid_t *groups;
	struct  passwd *pw;
	struct group *gr;
	ngroups = 50;

	groups = malloc(ngroups * sizeof (gid_t));
//obrtain group list
	getgrouplist(user, groups, &ngroups);

	//print out grouop info 
	for(j=0;j<ngroups;j++)
	{
        	gIDs[j] = groups[j]; 
       	 	gr = getgrgid(groups[j]);
        	if (gr != NULL)
        	printf("\n");
	}

//assigned group id  to user
	uID = gIDS[0];
//welcome message
	printf("Hello User : %d",uID);

}

//function to handlke the changing of ids between users to root
void userIDHandler(int uID)
{

    uid_t uid = getuid();
    uid_t gid = getgid();
    uid_t ueid = geteuid();
    uid_t geid = getegid();
//print out ids
    printf("User ID %d",uid);
    printf("Group ID %d",gid);
    printf("User  Effective ID %d",ueid);
    printf("Group Effective ID %d",geid); 


   //from root to user 
    setgroups(50,gIDS);
    setreuid(uID,uid);
    setregid(uID,gid);
    setreueid(uID);
    setregeid(uID);

//from user to root 
    int Rid = 0;
    setreuid(Rid);
    setregid(Rid,uid);
    setreueid(Rid,uid);
    setregeid(uID);

}
*/

