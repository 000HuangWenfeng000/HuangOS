#include "queue.h"

void init_queue(struct QUEUE *pQueue, int *buffer, int bufferSize)
{
	pQueue->front = 0;
	pQueue->rear = 0;
	pQueue->length = 0;
	pQueue->buffer = buffer; 
	pQueue->bufferSize = bufferSize;
}

void enQueue(struct QUEUE *pQueue, int data)
{
	if ((pQueue->rear + 1) % pQueue->bufferSize == pQueue->front)
		return;
	
	pQueue->buffer[pQueue->rear] = data;
	pQueue->rear = (pQueue->rear + 1) % pQueue->bufferSize;
	pQueue->length++;
}

int deQueue(struct QUEUE *pQueue)
{
	if (pQueue->front == pQueue->rear)
		return 0;

	int data = pQueue->buffer[pQueue->front];
	pQueue->front = (pQueue->front + 1) % pQueue->bufferSize;
	pQueue->length--;
	return data;
}

int isEmpty(struct QUEUE *pQueue)
{
	if (pQueue->front == pQueue->rear)
		return 1;
	return 0;
}

int queueLength(struct QUEUE *pQueue)
{
	return pQueue->length;
}

