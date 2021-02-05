#include <stdio.h>
#include <stdint.h>
#include <pthread.h>
#include <sys/sysinfo.h>

void* brute(void* thread_id) {
    printf("[%ld] Starting thread %ld\n", (intptr_t)thread_id, (intptr_t)thread_id);
    
    pthread_exit(NULL);
}

int main() {
    int cpu_count = get_nprocs();
    printf("[INFO] Running on %d threads\n", cpu_count);
    pthread_t threads[cpu_count];
    for (intptr_t i = 0; i < cpu_count; i++) {
        pthread_create(&threads[i], NULL, brute, (void*)i);
    }
    for (intptr_t i = 0; i < cpu_count; i++) {
        pthread_join(&threads[i], NULL);
    }
    return 0;
}