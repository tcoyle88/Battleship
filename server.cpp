/*
Turn-Based Game Server
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;


void handle_connect(int, int);
void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
     int sockfd, sock1, sock2, portno, i, pid;
     int connections=0;
     string chrts;
     
     socklen_t clilen;
     char buffer[256];
     struct sockaddr_in serv_addr, cli_addr;
     int n;
     if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     
     
	if (sockfd < 0) 
		error("ERROR opening socket");
    
		bzero((char *) &serv_addr, sizeof(serv_addr));
		portno = atoi(argv[1]);
		serv_addr.sin_family = AF_INET;
		serv_addr.sin_addr.s_addr = INADDR_ANY;
		serv_addr.sin_port = htons(portno);
		if (bind(sockfd, (struct sockaddr *) &serv_addr,
			sizeof(serv_addr)) < 0) 
				error("ERROR on binding");
              
		listen(sockfd,5);
		clilen = sizeof(cli_addr);
          
		while (1) {
			//connect two players
			sock1 = accept(sockfd,(struct sockaddr *) &cli_addr, &clilen);
         if (sock1 < 0) 
             error("ERROR on accept");
				n = write(sock1,"Please wait for opponent...",30);
				if (n < 0) error("ERROR writing to socket");
             sock2 = accept(sockfd,(struct sockaddr *) &cli_addr, &clilen);
         if (sock2 < 0) 
             error("ERROR on accept");
				n = write(sock2,"Please wait for opponent...",30);
				if (n < 0) error("ERROR writing to socket");
			//fork them to function
         pid = fork();
         if (pid < 0)
             error("ERROR on fork");
         if (pid == 0)  {
             close(sockfd);
             handle_connect(sock1,sock2);
             exit(0);
         }
         else {
         	close(sock1);
         	close(sock2);
         }
     } 
     
     close(sockfd);
     return 0; 
}


void handle_connect (int p1, int p2)
{
	int n;
	string p1name, p2name;
	char buffer[256];
   
	n = write(p1,"Connected to opponent!",30);
	if (n < 0) error("ERROR writing to socket");
	n = write(p1,"player1",30);
	if (n < 0) error("ERROR writing to socket");
	
	n = write(p2,"Connected to opponent! Wait for response....",50);
	if (n < 0) error("ERROR writing to socket");
	n = write(p2,"player2",30);
	if (n < 0) error("ERROR writing to socket");
    
    while(1){
		bzero(buffer,256);
		n = read(p1,buffer,255);
		if (n < 0) error("ERROR reading from socket");
		//printf("Here is the message: %s\n",buffer);
		n = write(p2,buffer,strlen(buffer));
		if (n < 0) error("ERROR writing to socket");
		
		bzero(buffer,256);
		n = read(p2,buffer,255);
		if (n < 0) error("ERROR reading from socket");
		//printf("Here is the message: %s\n",buffer);
		n = write(p1,buffer,strlen(buffer));
		if (n < 0) error("ERROR writing to socket");
	}
	
}