#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/wait.h>

#define SZ 4096

int main() {
    int shm_id;
    int *shm_ptr;
    int data;
    int ret_val;
    pid_t pid;

    // Create shared memory segment
    shm_id = shmget(IPC_PRIVATE, SZ, IPC_CREAT | 0666);
    if (shm_id < 0) {
        perror("shmget error");
        exit(EXIT_FAILURE);
    } else {
        printf("Shared memory created.\n");
        printf("Shared memory id: %d\n", shm_id);
    }

    // Attach to the shared memory segment
    shm_ptr = (int *)shmat(shm_id, NULL, 0);
    if (shm_ptr == (void *)-1) {
        perror("shmat error");
        shmctl(shm_id, IPC_RMID, NULL);  // Remove shared memory if shmat fails
        exit(EXIT_FAILURE);
    }
    printf("Shared memory attached at address: %p\n", (void*)shm_ptr);

    // Input data and write to shared memory
    printf("Enter the data: ");
    scanf("%d", &data);
    *shm_ptr = data;

    pid = fork();  // Create child process
    if (pid < 0) {
        perror("fork error");
        shmdt(shm_ptr);  // Detach shared memory before exiting
        shmctl(shm_id, IPC_RMID, NULL);  // Remove shared memory
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // Child process code
        int *child_shm_ptr = (int *)shmat(shm_id, NULL, 0);
        if (child_shm_ptr == (void *)-1) {
            perror("Child shmat error");
            exit(EXIT_FAILURE);
        }

        printf("Child: Shared memory attached at address: %p\n", (void*)child_shm_ptr);
        printf("Child: Data read from shared memory: %d\n", *child_shm_ptr);

        ret_val = shmdt(child_shm_ptr);  // Detach from shared memory
        if (ret_val == 0) {
            printf("Child: Shared memory successfully detached.\n");
        } else {
            perror("Child shmdt error");
        }

        exit(EXIT_SUCCESS);
    } else {
        // Parent process code
        wait(NULL);  // Wait for the child process to finish

        ret_val = shmdt(shm_ptr);  // Detach from shared memory
        if (ret_val == 0) {
            printf("Parent: Shared memory successfully detached.\n");
        } else {
            perror("Parent shmdt error");
        }

        ret_val = shmctl(shm_id, IPC_RMID, NULL);  // Remove shared memory
        if (ret_val == 0) {
            printf("Parent: Shared memory segment removed.\n");
        } else {
            perror("shmctl IPC_RMID error");
        }
    }

    return 0;
}

