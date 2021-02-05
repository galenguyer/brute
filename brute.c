#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <limits.h>
#include <pthread.h>
#ifdef __linux__
    #include <sys/sysinfo.h>
#endif
#include "md5.h"

#ifndef HOST_NAME_MAX
    #define HOST_NAME_MAX 32
#endif

volatile int best_prefix = 5;
volatile int best_suffix = 5;

char* get_hostname() {
    char* hostname = malloc(HOST_NAME_MAX + 1);
    gethostname(hostname, HOST_NAME_MAX + 1);
    return hostname;
}

void* brute(void* thread_id) {
    printf("[%s->%ld] Starting thread %ld\n", get_hostname(), (intptr_t)thread_id, (intptr_t)thread_id);
    char* buffer = malloc(sizeof(char) * 64);
    sprintf(buffer, "%d", rand());
    buffer = md5(buffer);
    while (1)
    {
        char* old_buffer = buffer;
        buffer = md5(buffer);
        int len = strlen(buffer);
        for (int i = 0; i < len; i++){
            if (old_buffer[i] != buffer[i]) {
                if (i > best_prefix) {
                    printf("[%s->%ld] new best prefix match: %d characters\n", get_hostname(), (intptr_t)thread_id, i);
                    printf("[%s->%ld] %s -> %s\n", get_hostname(), (intptr_t)thread_id, old_buffer, buffer);
                    best_prefix = i;
                }
                break;
            }
        }
        for (int i = len-1; i > 0; i--){
            if (old_buffer[i] != buffer[i]) {
                if (len-i > best_suffix) {
                    printf("[%s->%ld] new best suffix match: %d characters\n", get_hostname(), (intptr_t)thread_id, len-i-1);
                    printf("[%s->%ld] %s -> %s\n", get_hostname(), (intptr_t)thread_id, old_buffer, buffer);
                    best_suffix = len-i;
                }
                break;
            }
        }
        free(old_buffer);
    }
    
    pthread_exit(NULL);
}

int main() {
    srand(time(0));
    const int CPU_COUNT = get_nprocs();
    printf("[%s->INFO] Running on %d threads\n", get_hostname(), CPU_COUNT);
    pthread_t threads[CPU_COUNT];
    for (intptr_t i = 0; i < CPU_COUNT; i++) {
        pthread_create(threads + i, NULL, brute, (void*)i);
    }
    for (intptr_t i = 0; i < CPU_COUNT; i++) {
        pthread_join(threads[i], NULL);
    }
    return 0;
}
