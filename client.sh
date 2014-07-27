echo "IP Address of Server:"
read ip
echo "Port Number:"
read port
echo "Start [y/n]:"
read op
if [ $op = "y" ] ; then
	echo "hi"
	gcc -o client client.c -L/usr/X11R6/lib -lX11
	./client $ip $port
fi
	
