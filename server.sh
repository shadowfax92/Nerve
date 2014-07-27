echo "Port Number:"
read port
echo "Start [y/n]:"
read op
if [ $op = "y" ] ; then
	gcc -o server server.c 
	./server $port
fi
	
