#include <stdio.h>
#include <sys/sysinfo.h>

int main() {
    printf("[INFO] Running on %d threads\n", get_nprocs());
    return 0;
}