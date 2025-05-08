 #include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define ARRAY_SIZE 200000
#define MAX_THREADS 20

int arr[ARRAY_SIZE];
long long thread_sum[MAX_THREADS]; 

typedef struct {
    int thread_id;
    int num_threads;
} ThreadArgs;

void* sum_each_thread(void* arg) {
    ThreadArgs* args = (ThreadArgs*)arg;
    int size = ARRAY_SIZE / args->num_threads;  
    int start = args->thread_id * size;
    int end = (args->thread_id == args->num_threads - 1) ? ARRAY_SIZE : start + size;

    long long sum = 0;
    for (int i = start; i < end; i++) {
        sum += arr[i];
    }

    thread_sum[args->thread_id] = sum;
    pthread_exit(NULL);
}

int main() {
    srand(time(NULL));

    for (int i = 0; i < ARRAY_SIZE; i++) {
        arr[i] = rand() % 150;
    }

    pthread_t threads[MAX_THREADS];
    ThreadArgs args[MAX_THREADS];
    long long total_sum;

    int thread_counts[] = {1, 5, 10, 15, 20};  
    for (int test = 0; test < 5; test++) {
        int num_threads = thread_counts[test];
        total_sum = 0;

        struct timespec start_time, end_time;
        clock_gettime(CLOCK_MONOTONIC, &start_time);

       
        for (int i = 0; i < num_threads; i++) {
            args[i].thread_id = i;
            args[i].num_threads = num_threads;
            pthread_create(&threads[i], NULL, sum_each_thread, &args[i]);
        }

        for (int i = 0; i < num_threads; i++) {
            pthread_join(threads[i], NULL);
            total_sum += thread_sum[i];
        }

        clock_gettime(CLOCK_MONOTONIC, &end_time);
        double elapsed_time = (end_time.tv_sec - start_time.tv_sec) + 
                              (end_time.tv_nsec - start_time.tv_nsec) / 1e9;

        printf("Threads: %2d | Time: %.5f sec | Sum: %lld\n", 
               num_threads, elapsed_time, total_sum);
    }

    return 0;
}
