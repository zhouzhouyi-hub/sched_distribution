#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

typedef unsigned long long u64;

// Structure to store scheduling data
struct sched_data {
    u64 prime;
};

// Function to get the current time in microseconds
u64 get_time_us() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (u64)(ts.tv_sec * 1000000 + ts.tv_nsec / 1000);
}

// The provided run_loops function
static u64 run_loops(struct sched_data *data, u64 loops) {
    u64 start = get_time_us();
    u64 end;
    u64 i;
    u64 prime = data->prime;
    u64 cnt = 2;
    u64 result;

    for (i = 0; i < loops; i++) {
        if (cnt > prime / 2) {
            data->prime = prime;
            prime++;
            cnt = 2;
        }
        result = prime / cnt;
        if (result * cnt == prime) {
            prime++;
            cnt = 2;
        } else
            cnt++;
    }

    // Memory barrier
    asm("":::"memory");

    end = get_time_us();
    return end - start;
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <number_of_loops_per_round> <total_runtime_us> <output_file>\n", argv[0]);
        return 1;
    }

    u64 loops_per_round = strtoull(argv[1], NULL, 10);
    u64 total_runtime_us = strtoull(argv[2], NULL, 10);
    const char *output_file = argv[3];

    struct sched_data data;
    data.prime = 2;  // Starting prime value

    FILE *file = fopen(output_file, "w");
    if (file == NULL) {
        perror("Failed to open output file");
        return 1;
    }

    u64 elapsed_time = 0;
    int round = 0;

    while (elapsed_time < total_runtime_us) {
        u64 round_time = run_loops(&data, loops_per_round);
        fprintf(file, "%llu\n", round_time);
        elapsed_time += round_time;
    }

    fclose(file);
    printf("Results written to %s\n", output_file);

    return 0;
}
