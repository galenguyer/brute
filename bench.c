#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>
#include <sys/time.h> 
#include "md5.h"

void* bench(void* iters) {
    const intptr_t iterations = (intptr_t)iters;
    char* buffer = malloc(sizeof(char) * 64);
    sprintf(buffer, "%d", rand());
    buffer = md5(buffer);
    for (int i = 0; i < iterations; i++)
    {
        char* old_buffer = buffer;
        buffer = md5(buffer);
        int len = strlen(buffer);
        for (int i = 0; i < len; i++){
            if (old_buffer[i] != buffer[i]) {
                break;
            }
        }
        for (int i = len-1; i > 0; i--){
            if (old_buffer[i] != buffer[i]) {
                break;
            }
        }
        free(old_buffer);
    }
}

int main() {
    srand(time(0));
    const int CPU_COUNT = get_nprocs();

    printf("[bench->INFO] Running on 1 thread\n");
    
    struct timeval st_start, st_end;
    gettimeofday(&st_start, NULL);
    bench((void*) 10000000);
    gettimeofday(&st_end, NULL);
    
    printf("[bench->INFO] Single threaded test took %lds%ldms\n", 
        (st_end.tv_sec - st_start.tv_sec),
        (((st_end.tv_sec*1000000+st_end.tv_usec)-(st_start.tv_sec*1000000+st_start.tv_usec))/1000)%1000);
    long int st_diff_ms = (((st_end.tv_sec*1000000+st_end.tv_usec)-(st_start.tv_sec*1000000+st_start.tv_usec))/1000);
    printf("[bench->INFO] Single threaded rate: %0.2fMh/s\n", 10000000/(float)st_diff_ms);

    printf("[bench->INFO] Running on %d threads\n", CPU_COUNT);
    struct timeval mt_start, mt_end;
    gettimeofday(&mt_start, NULL);
    pthread_t threads[CPU_COUNT];
    for (intptr_t i = 0; i < CPU_COUNT; i++) {
        pthread_create(threads + i, NULL, bench, (void*)(10000000/CPU_COUNT));
    }
    for (intptr_t i = 0; i < CPU_COUNT; i++) {
        pthread_join(threads[i], NULL);
    }
    gettimeofday(&mt_end, NULL);
    printf("[bench->INFO] Multithreaded test took %lds%ldms\n", 
        (mt_end.tv_sec - mt_start.tv_sec),
        (((mt_end.tv_sec*1000000+mt_end.tv_usec)-(mt_start.tv_sec*1000000+mt_start.tv_usec))/1000)%1000);

    long int mt_diff_ms = (((mt_end.tv_sec*1000000+mt_end.tv_usec)-(mt_start.tv_sec*1000000+mt_start.tv_usec))/1000);
    printf("[bench->INFO] Multithreaded rate: %0.2fMh/s\n", 10000000/(float)mt_diff_ms);
    
    return 0;
}
