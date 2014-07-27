#include<X11/Xlib.h> 
#include<stdio.h>
#include<stdlib.h>

char map[128];
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
	return;
}
int main() {
Display *display = XOpenDisplay(NULL);
KeySym k;
int revert_to,temp=0;
Window window;
XEvent event;

XGetInputFocus(display, &window, &revert_to);
XSelectInput(display, window, KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask);
initmap();
while(1)
{
XNextEvent(display,&event);
/*switch (event.type) {

case KeyPress : printf("Key Pressed\n"); break;
//case KeyRelease : break;
//case EnterNotify : printf("Enter\n"); break;
}*/
temp=event.xkey.keycode;
printf("Key code:%c = %d\n",map[temp],temp);
//printf("%d\n",event.xkey.keycode);
}
XCloseDisplay(display);
return 0;
}
