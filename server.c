#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
     int sockfd, newsockfd, portno;
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
     newsockfd = accept(sockfd, 
                 (struct sockaddr *) &cli_addr, 
                 &clilen);
     if (newsockfd < 0) 
          error("ERROR on accept");
          
     while(1)
     {
		 
		 char str[100]="xdotool mousemove ";
		 char xyz[100]="xdotool click 1";
		 char typ[100]="xdotool type ";
		 char enter[100]="xdotool key KP_Enter";
		 
     bzero(buffer,256);
     n = read(newsockfd,buffer,255);
     if (n < 0) 
		error("ERROR reading from socket");
		
     //printf("\n%s\n",buffer);
     
     if(buffer[0]=='!')
     {
		 printf("mouseclick%s\n",xyz);
		 system(xyz);
	 }
	 else if(buffer[0]=='*')
	 {	 
		 //printf("Key typed:%c\n",buffer[1]);
		 //strncat(typ,buffer[1]);
		
		if(buffer[1]=='\n')
		{
			printf("Key typed:Enter\n",buffer[1]);
			system(enter);
			//buffer[1]='\0';
		}
		else if(buffer[1]=='\e')
		{
			printf("Key typed:Escape\n",buffer[1]);
			exit(0);
			//buffer[1]='\0';
		}
		else if(buffer[1]=='\b')
		{
			printf("Key typed:Backspace\n",buffer[1]);
			system("xdotool key BackSpace");
		}
		else if(buffer[1]=='\001')
		{
			printf("Key typed:space\n",buffer[1]);
			system("xdotool key KP_Space");
		}
		else{
		printf("Key typed:%c\n",buffer[1]);
		 typ[strlen(typ)]=buffer[1];
		 typ[strlen(typ)+1]='\0';
		 //printf("\ntype:%s\n",typ);
		 system(typ);
		}
	 }
	 else
	 {
		 
		 strncat(str,buffer,strlen(buffer)-2);
		 printf("movemouse:%s\n",str);
		 system(str);
	 }
     

	}
     close(newsockfd);
     close(sockfd);
     return 0; 
}
