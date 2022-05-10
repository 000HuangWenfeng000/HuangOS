#include <stdio.h>
#include "queue.h"


int main()
{
	// void init_queue(struct QUEUE *pQueue, int *buffer, int bufferSize);
	int bufferSize = 16;
	int buffer[bufferSize];
	struct QUEUE queue;
	init_queue(&queue, buffer, bufferSize);
	printf("init_queue() \n");

	// int isEmpty(struct QUEUE *pQueue);
	int flag;
	flag = isEmpty(&queue);
	printf("isEmpty : %d \n", flag);
	// int queueLength(struct QUEUE *pQueue);
	int length;
	length = queueLength(&queue);
	printf("queueLength : %d \n", length);

	//void enQueue(struct QUEUE *pQueue, int data);
	for (int i = 0; i < bufferSize + 5; ++i) {
		enQueue(&queue, i);
		printf("enQueue() : %d \n", i);
	}

	flag = isEmpty(&queue);
	printf("isEmpty : %d \n", flag);
	length = queueLength(&queue);
	printf("queueLength : %d \n", length);

	// int deQueue(struct QUEUE *pQueue);
	int data;
	for (int i = 0; i < bufferSize + 5; ++i) {
		data = deQueue(&queue);
		printf("deQueue() : %d \n", data);
	}

	// int isEmpty(struct QUEUE *pQueue);
	flag = isEmpty(&queue);
	printf("isEmpty : %d \n", flag);

	// int queueLength(struct QUEUE *pQueue);
	length = queueLength(&queue);
	printf("queueLength : %d \n", length);

	return 0;
}