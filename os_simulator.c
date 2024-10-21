#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/shm.h>

#define BUFFER_SIZE 1024

pthread_mutex_t mutex;      // Mutex for synchronization
sem_t semaphore;            // Semaphore for process coordination

// Shared memory structure
struct shared_data {
    char message[BUFFER_SIZE];
};

// Function to simulate a process using pthreads
void* process_routine(void* arg) {
    char* process_name = (char*)arg;
    
    // Simulating critical section using a mutex
    pthread_mutex_lock(&mutex);
    printf("%s is in the critical section\n", process_name);
    
    // Critical section work (simulating)
    sleep(1);
    
    printf("%s is leaving the critical section\n", process_name);
    pthread_mutex_unlock(&mutex);
    
    return NULL;
}

// Function to simulate inter-process communication (IPC) using pipes
void simulate_ipc_with_pipes() {
    int pipefd[2];
    pid_t pid;
    char write_msg[BUFFER_SIZE] = "Hello from parent process!";
    char read_msg[BUFFER_SIZE];

    if (pipe(pipefd) == -1) {
        perror("Pipe failed");
        exit(EXIT_FAILURE);
    }

    pid = fork();
    if (pid < 0) {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    }

    if (pid > 0) {  // Parent process
        close(pipefd[0]);  // Close the reading end
        write(pipefd[1], write_msg, strlen(write_msg) + 1);
        close(pipefd[1]);  // Close the writing end
        wait(NULL);        // Wait for child process to finish
    } else {  // Child process
        close(pipefd[1]);  // Close the writing end
        read(pipefd[0], read_msg, BUFFER_SIZE);
        printf("Child process received message: %s\n", read_msg);
        close(pipefd[0]);  // Close the reading end
    }
}

// Function to simulate IPC using shared memory
void simulate_ipc_with_shared_memory() {
    int shm_id;
    struct shared_data* shared_mem;
    
    // Create shared memory segment
    shm_id = shmget(IPC_PRIVATE, sizeof(struct shared_data), IPC_CREAT | 0666);
    if (shm_id < 0) {
        perror("shmget failed");
        exit(EXIT_FAILURE);
    }
    
    // Attach the shared memory segment to parent process
    shared_mem = (struct shared_data*)shmat(shm_id, NULL, 0);
    if (shared_mem == (void*)-1) {
        perror("shmat failed");
        exit(EXIT_FAILURE);
    }
    
    pid_t pid = fork();
    if (pid < 0) {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    }
    
    if (pid > 0) {  // Parent process
        strcpy(shared_mem->message, "Hello from shared memory!");
        wait(NULL);  // Wait for child process to finish
        
        // Detach shared memory
        shmdt(shared_mem);
        shmctl(shm_id, IPC_RMID, NULL);  // Remove shared memory
    } else {  // Child process
        sleep(1);  // Allow parent to write first
        printf("Child process read from shared memory: %s\n", shared_mem->message);
        
        // Detach shared memory
        shmdt(shared_mem);
        exit(EXIT_SUCCESS);
    }
}

int main() {
    // Initialize mutex and semaphore
    pthread_mutex_init(&mutex, NULL);
    sem_init(&semaphore, 0, 1);
    
    // Create multiple threads to simulate processes
    pthread_t thread1, thread2;
    
    pthread_create(&thread1, NULL, process_routine, (void*)"Process 1");
    pthread_create(&thread2, NULL, process_routine, (void*)"Process 2");
    
    // Wait for threads to finish
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    
    // Simulate IPC using pipes
    simulate_ipc_with_pipes();
    
    // Simulate IPC using shared memory
    simulate_ipc_with_shared_memory();
    
    // Clean up resources
    pthread_mutex_destroy(&mutex);
    sem_destroy(&semaphore);
    
    return 0;
}
