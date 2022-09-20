#pragma once
#define MIN_BRIGHT 0
#define MAX_BRIGHT 10

#define MIN_SPEED 1
#define MAX_SPEED 100
#define SCROLL_SPEED 20


void createTasks();
void toggleCount(void *parameter);
void callGetData(void *parameter);

void runMatrix(void *parameter);
 void runServer(void *parameter);
 void SetMatrixSpeed(int speed);
void displayCoinData();