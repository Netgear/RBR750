/************
 *
 * Filename:    dalh_queue.h
 * Purpose:     function and defines used for dalh process queue
 * Copyright:   (c) Netgear Inc.
 * 				2019 All rights reserved
 * Author:      @ VVDN TECHNOLOGIES
 *
 *************/

/* Define to prevent recursive inclusion---------------------*/

#ifndef _DALH_QUEUE_H_
#define _DALH_QUEUE_H_

#include <dalh_def.h>

/* define for the queue */
#define MAX_DALH_QUEUE_SIZE BUFFER_SIZE_32
/* Global variables */
struct Queue ArrQueue[BUFFER_SIZE_32];
int head, tail; // Declare global indices to head and tail of queue

/* Function Prototypes for Fing Queue */
void dalhInitQueue();  // Initialize the Fing queue
void dalhClearQueue(); // Remove all items from the Fing queue
int dalhEnqueue(struct Queue *pNtfData, uint32_t fId); //Insert data in the dalh queue
int dalhDequeue(uint32_t fId);      // Remove an item from the Fing queue
int isDalhQueueEmpty(); // Return true if Fing queue is empty
int isDalhQueueFull();  // Return true if FIng queue is full

#endif /* _DALH_QUEUE_H_ */
