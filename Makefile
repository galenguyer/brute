brute: md5.c brute.c
	gcc -std=c99 -D_XOPEN_SOURCE=500 -Wall -pthread brute.c md5.c -O2 -o brute

bench: md5.c bench.c
	gcc -std=c99 -D_XOPEN_SOURCE=500 -Wall -pthread bench.c md5.c -O2 -o bench

debug: md5.c brute.c
	gcc -ggdb -std=c99 -D_XOPEN_SOURCE=500 -pthread -Wall brute.c md5.c -o brute
