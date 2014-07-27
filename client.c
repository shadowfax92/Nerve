#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <stdio.h>
#include <assert.h>
#include <X11/Xlib.h>

char map[128];
int toggle=0;
void error(const char *msg)
{
    perror(msg);
    exit(0);
}
void initmap()
{
	map[38]='a';
	map[56]='b';
	map[54]='c';
	map[40]='d';
	map[26]='e';
	map[41]='f';
	map[42]='g';
	map[43]='h';
	map[31]='i';
	map[44]='j';
	map[45]='k';
	map[46]='l';
	map[58]='m';
	map[57]='n';
	map[32]='o';
	map[33]='p';
	map[24]='q';
	map[27]='r';
	map[39]='s';
	map[28]='t';
	map[30]='u';
	map[55]='v';
	map[25]='w';
	map[53]='x';
	map[29]='y';
	map[52]='z';
	
	map[65]='\001';//space
	map[36]='\n';//enter
	map[37]='\003';//left control
	map[64]='\004';//left alt
	map[133]='\002';//windows key
	map[23]='\t';//tab
	map[9]='\e';//esc
	map[22]='\b';
	map[60]='.';
	map[59]=',';
	map[62]=';';


	map[10]='1';
	map[11]='2';
	map[12]='3';
	map[13]='4';
	map[14]='5';
	map[15]='6';
	map[16]='7';
	map[17]='8';
	map[18]='9';
	map[19]='0';
	return;
}
int main(int argc, char *argv[])
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    FILE *fp;

    char buffer[256];
	Display *d;
	Window root;
	int revert_to;
	
	Window temp_window;
	int temp_revert_to;

	d = XOpenDisplay(NULL);
	assert(d);

	root = DefaultRootWindow(d);

	XGrabPointer(d, root, False, ButtonPressMask | ButtonReleaseMask |
		     PointerMotionMask, GrabModeAsync, GrabModeAsync, None,
		     None, CurrentTime);
	XEvent ev;
		XGetInputFocus(d, &root, &revert_to);
		XSelectInput(d, root, KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask);
		initmap();

    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }

    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0) 
        error("ERROR opening socket");
    server = gethostbyname(argv[1]);

    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;

    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);

    if (connect(sockfd,(struct sockaddr *) &serv_addr,
    sizeof(serv_addr)) < 0) 
        error("ERROR connecting");

	int test_map;
    while(1)
    {
   
		int buffer_len;
		if(toggle==1)//if flag is one write or else don't write
		{
			XGrabPointer(d, root, False, ButtonPressMask | ButtonReleaseMask |
		     PointerMotionMask, GrabModeAsync, GrabModeAsync, None,
		     None, CurrentTime);
			system("xdotool getmouselocation | grep -o [0-9]* | tr '\n' ' ' > loc");
			
			fp=fopen("loc","r");
			bzero(buffer,256);
			fgets(buffer,100,fp);
			fclose(fp);
			
			n = write(sockfd,buffer,strlen(buffer));
		}
		else 
		{
			XUngrabPointer(d,CurrentTime);
		}
		bzero(buffer,256);	
		XNextEvent(d, &ev);  
	
		switch (ev.type) { 
		case ButtonPress:
				buffer[0]='!';
				buffer[1]='1';
		        break;
		case ButtonRelease:
		        break;
		case MotionNotify:
		        break;
		case KeyPress : 
						test_map=ev.xkey.keycode;
						//printf("Key code:%c = %d\n",map[test_map],test_map);
						if(test_map==133)
						{
							toggle=!toggle;
						}
						buffer[0]='*';
						if(test_map!=9)
							buffer[1]=map[test_map];
						else
						{
							buffer[1]=map[test_map];
							if(toggle==1)//if flag is one write or else don't write
							{
								n = write(sockfd,buffer,strlen(buffer));
								exit(0);
							}
						}
						
						break;
		case KeyRelease : break;
		case EnterNotify :break;
		default:
		        printf("Unknown event...\n");
		} 
		if(toggle==1)//if flag is one write or else don't write
		{
		n = write(sockfd,buffer,strlen(buffer));
		//printf("\n%s\n",buffer);
		}
		//else
		//{
		//	XGetInputFocus(&d,&temp_window,&temp_revert_to);
		//	XSendEvent(d,temp_window,False,KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask, &ev);
		//}
		
	}
 	XCloseDisplay(d);
	close(sockfd);
    return 0;
}
