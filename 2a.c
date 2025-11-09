#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
void bubbleSort(int arr[], int n) {
int i, j, temp;
for (i = 0; i < n-1; i++) {
for (j = 0; j < n-1-i; j++) {
if (arr[j] > arr[j+1]) {
temp = arr[j]; arr[j] = arr[j+1]; arr[j+1] = temp;
}
}
}
}
void insertionSort(int arr[], int n) {
int i, key, j;
for (i = 1; i < n; i++) {
key = arr[i];
j = i -1;
while (j >=0 && arr[j] > key) {
arr[j+1] = arr[j];
j--;
}
arr[j+1] = key;
}
}
void printArray(const char* prefix, int arr[], int n) {
printf("%s", prefix);
for (int i=0; i<n; i++) {
printf("%d ", arr[i]);
}
printf("\n");
}
int main() {
int n;
printf("Enter number of integers: ");
scanf("%d", &n);
int *arr = malloc(n * sizeof(int));
int *arr_copy = malloc(n * sizeof(int));
printf("Enter %d integers:\n", n);
for(int i=0; i<n; i++) {
scanf("%d", &arr[i]);
arr_copy[i] = arr[i];
}
pid_t pid = fork();
if (pid < 0) {
perror("fork failed");
free(arr);
free(arr_copy);
exit(1);
}
if (pid == 0) {
// Child process
printf("Child Process (PID=%d): Sorting using Insertion Sort\n", getpid());
// Sorting child copy using insertion sort
insertionSort(arr_copy, n);
printArray("Child sorted array: ", arr_copy, n);
// Simulate orphan process: sleep longer than parent to let parent exit first
printf("Child going to sleep to simulate orphan...\n");
sleep(10); // during this sleep, parent should have exited -> orphan
printf("Child woke up and exiting.\n");
free(arr);
free(arr_copy);
exit(0);
} else {
// Parent process
printf("Parent Process (PID=%d): Sorting using Bubble Sort\n", getpid());
bubbleSort(arr, n);
printArray("Parent sorted array: ", arr, n);
// Simulate zombie: sleep before wait to keep child terminated but not waited on
printf("Parent sleeping for 5 seconds before waiting for child to demonstrate zombie.\n");
sleep(5);
// Wait for child to avoid zombie
int status;
wait(&status);
printf("Parent: Child has finished.\n");
// Parent exits first; child still sleeping to show orphan
printf("Parent exiting now.\n");
free(arr);
free(arr_copy);
exit(0);
}
return 0;
}
