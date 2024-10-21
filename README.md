# Operating System Simulator
A multi-threaded operating system simulator built using POSIX threads (pthreads), featuring inter-process communication (pipes and shared memory) and kernel-level synchronization (mutexes, semaphores) to simulate concurrent execution and manage process coordination.

## Features

- **Multithreading**: Simulates concurrent process execution using POSIX threads.
- **Inter-process Communication (IPC)**: 
  - **Pipes**: Allows data exchange between parent and child processes.
  - **Shared Memory**: Enables shared data access between processes.
- **Kernel-level Synchronization**: Implements mutexes and semaphores for mutual exclusion and preventing race conditions.

## Prerequisites

- GCC compiler (or any C compiler)
- POSIX-compliant system (e.g., Linux)

## Compilation

Use the following command to compile the program:

```bash
gcc -pthread -o os_simulator os_simulator.c
```

### Running the Program

After compilation, you can run the executable:

```bash
./os_simulator
```

## Code Overview

- **`process_routine`**: Simulates processes using threads, demonstrating the use of mutexes for synchronization.
- **`simulate_ipc_with_pipes`**: Demonstrates inter-process communication using pipes.
- **`simulate_ipc_with_shared_memory`**: Demonstrates shared memory communication between parent and child processes.

## Example Output

```
Process 1 is in the critical section
Process 1 is leaving the critical section
Process 2 is in the critical section
Process 2 is leaving the critical section
Child process received message: Hello from parent process!
Child process read from shared memory: Hello from shared memory!
```
