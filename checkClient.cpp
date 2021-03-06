#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h> 
class initClient
{
public:
	initClient(char* serverIP);
	~initClient();
    int Recv(char* filename);
private:

    int sockfd, n;
	char recvBuff[1024];
	struct sockaddr_in serv_addr; 
    int port;
    char* serverIP;

};

initClient::initClient(char* serverIP)	//constructor
{
    this->sockfd = 0;
    this->n = 0;
	this->port=55556;
	this->serverIP=serverIP;
    
}

initClient::~initClient()	//destructor
{

}

int initClient::Recv(char* filename)
{
    
	printf("Connecting...\n");
	memset(this->recvBuff, '0',sizeof(this->recvBuff));
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("\n Error : Could not create socket \n");
		return 1;
	} 

	memset(&(this->serv_addr), '0', sizeof(this->serv_addr)); 

	this->serv_addr.sin_family = AF_INET;
	this->serv_addr.sin_port = htons(this->port); 

	if(inet_pton(AF_INET, this->serverIP, &(this->serv_addr.sin_addr))<=0)
	{
		printf("\nERROR: inet_pton()\n");
		return 1;
	} 

	if( connect(this->sockfd, (struct sockaddr *)&(this->serv_addr), sizeof(this->serv_addr)) < 0)
	{
		printf("\n Error : Connect() \n");
		return 1;
	} 
	printf("Connected\n");
    unsigned int fileSize= -1;
	if( (n = read(this->sockfd, (char*)&fileSize, sizeof(int))) > 0)
	{
		printf("File Size for the request file is %d\n",fileSize);
	}else{
		printf("\nClient Read error \n");
	} 
	char * fileArray_c = (char*)calloc(fileSize+10, sizeof(char));


	if( (n = read(this->sockfd, fileArray_c, fileSize+5 )) > 0)
	{
		
		//printf("File Size for the request file is %d\n",fileSize);
	}else{
		printf("\nClient Read error \n");
	} 
	

	FILE *fp = fopen(filename,"w");
        if(fp==NULL)
        {
            printf("File opern error");
            return -1;  
        }

            int nread = fwrite(fileArray_c,1,n,fp);
            printf("Bytes read %d \n", nread);        

            if (nread < fileSize)
            {
                if (feof(fp))
                    printf("End of file\n");
                if (ferror(fp))
                    printf("Error reading\n");

            }



	fclose(fp);

	
	free(fileArray_c);
	return 0;

}


