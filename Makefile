brute: md5.c brute.c
	gcc -std=c99 -Wall brute.c md5.c -O2 -o brute

debug: md5.c brute.c
	gcc -ggdb -std=c99 -Wall brute.c md5.c -o brute
