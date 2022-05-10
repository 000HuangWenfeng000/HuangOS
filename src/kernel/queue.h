#ifndef __QUEUE_H__
#define __QUEUE_H__

struct QUEUE {
	int front;
	int rear;
	int length;
	int *buffer;
	int bufferSize;
};

void init_queue(struct QUEUE *pQueue, int *buffer, int bufferSize);
void enQueue(struct QUEUE *pQueue, int data);
int deQueue(struct QUEUE *pQueue);
int isEmpty(struct QUEUE *pQueue);
int queueLength(struct QUEUE *pQueue);

#endif // __QUEUE_H__