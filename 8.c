#include <stdio.h>
#include <stdlib.h>
int SSTF();
int SCAN();
int CLOOK();
int main() {
int ch, YN = 1;
while (YN == 1) {
printf("\n\n\t*********** MENU ***********");
printf("\n\n\t1: SSTF\n\t2: SCAN\n\t3: CLOOK\n\t4: EXIT");
printf("\n\n\tEnter your choice: ");
scanf("%d", &ch);
switch (ch) {
case 1:
SSTF();
break;
case 2:
SCAN();
break;
case 3:
CLOOK();
break;
case 4:
exit(0);
default:
printf("Invalid choice!\n");
}
printf("\n\n\tDo you want to continue? IF YES PRESS 1\n\tIF NO PRESS 0: ");
scanf("%d", &YN);
}
return 0;
}
// SSTF Algorithm
int SSTF() {
int RQ[100], i, n, TotalHeadMovement = 0, initial, count = 0;
printf("Enter the number of Requests: ");
scanf("%d", &n);
printf("Enter the Requests sequence:\n");
for (i = 0; i < n; i++) {
scanf("%d", &RQ[i]);
}
printf("Enter initial head position: ");scanf("%d", &initial);
while (count != n) {
int min = 10000, d, index = -1;
for (i = 0; i < n; i++) {
d = abs(RQ[i] - initial);
if (min > d && RQ[i] != -1) {
min = d;
index = i;
}
}
TotalHeadMovement += min;
initial = RQ[index];
RQ[index] = -1; // Mark as serviced
count++;
}
printf("Total head movement is %d\n", TotalHeadMovement);
return 0;
}
// SCAN Algorithm
int SCAN() {
int RQ[100], i, j, n, TotalHeadMovement = 0, initial, size, move;
printf("Enter the number of Requests: ");
scanf("%d", &n);
printf("Enter the Requests sequence:\n");
for (i = 0; i < n; i++) {
scanf("%d", &RQ[i]);
}
printf("Enter initial head position: ");
scanf("%d", &initial);
printf("Enter total disk size: ");
scanf("%d", &size);
printf("Enter the head movement direction (high=1, low=0): ");
scanf("%d", &move);
// Sort the request queue
for (i = 0; i < n - 1; i++) {
for (j = 0; j < n - i - 1; j++) {
if (RQ[j] > RQ[j + 1]) {
int temp = RQ[j];
RQ[j] = RQ[j + 1];
RQ[j + 1] = temp;
}
}
}
int index = 0;
for (i = 0; i < n; i++) {
if (initial < RQ[i]) {
index = i;
break;}
}
if (move == 1) { // Moving towards higher end
for (i = index; i < n; i++) {
TotalHeadMovement += abs(RQ[i] - initial);
initial = RQ[i];
}
TotalHeadMovement += abs((size - 1) - initial);
initial = size - 1;
for (i = index - 1; i >= 0; i--) {
TotalHeadMovement += abs(RQ[i] - initial);
initial = RQ[i];
}
} else { // Moving towards lower end
for (i = index - 1; i >= 0; i--) {
TotalHeadMovement += abs(RQ[i] - initial);
initial = RQ[i];
}
TotalHeadMovement += abs(initial - 0);
initial = 0;
for (i = index; i < n; i++) {
TotalHeadMovement += abs(RQ[i] - initial);
initial = RQ[i];
}
}
printf("Total head movement is %d\n", TotalHeadMovement);
return 0;
}
// C-LOOK Algorithm
int CLOOK() {
int RQ[100], i, j, n, TotalHeadMovement = 0, initial, size, move;
printf("Enter the number of Requests: ");
scanf("%d", &n);
printf("Enter the Requests sequence:\n");
for (i = 0; i < n; i++) {
scanf("%d", &RQ[i]);
}
printf("Enter initial head position: ");
scanf("%d", &initial);
printf("Enter total disk size: ");
scanf("%d", &size);
printf("Enter the head movement direction (high=1, low=0): ");
scanf("%d", &move);
// Sort the request queue
for (i = 0; i < n - 1; i++) {
for (j = 0; j < n - i - 1; j++) {
if (RQ[j] > RQ[j + 1]) {
int temp = RQ[j];
RQ[j] = RQ[j + 1];RQ[j + 1] = temp;
}
}
}
int index = 0;
for (i = 0; i < n; i++) {
if (initial < RQ[i]) {
index = i;
break;
}
}
if (move == 1) { // Moving towards higher end
for (i = index; i < n; i++) {
TotalHeadMovement += abs(RQ[i] - initial);
initial = RQ[i];
}
for (i = 0; i < index; i++) {
TotalHeadMovement += abs(RQ[i] - initial);
initial = RQ[i];
}
} else { // Moving towards lower end
for (i = index - 1; i >= 0; i--) {
TotalHeadMovement += abs(RQ[i] - initial);
initial = RQ[i];
}
for (i = n - 1; i >= index; i--) {
TotalHeadMovement += abs(RQ[i] - initial);
initial = RQ[i];
}
}
printf("Total head movement is %d\n", TotalHeadMovement);
return 0;
}
