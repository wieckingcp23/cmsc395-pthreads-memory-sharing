#/bin/bash
count=0
while [ $count -lt 25 ]; do
	sleep 1;
	echo 7 | nc localhost 8889;
	count=$((count + 1));
done
