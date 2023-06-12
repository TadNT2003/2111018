
#include <stdlib.h>
#include "queue.h"
#include <pthread.h>

/* Remember to initilize the queue before using it */
void initialize_queue(struct pqueue_t * q) {
	q->head = q->tail = NULL;
	pthread_mutex_init(&q->lock, NULL);
}

/* Return non-zero if the queue is empty */
int empty(struct pqueue_t * q) {
	return (q->head == NULL);
}

/* Get PCB of a process from the queue (q).
 * Return NULL if the queue is empty */
struct pcb_t * de_queue(struct pqueue_t * q) {
	struct pcb_t * proc = NULL;
	// TODO: return q->head->data and remember to update the queue's head
	// and tail if necessary. Remember to use 'lock' to avoid race
	// condition
	
	// YOUR CODE HERE

	if (q->head == NULL) return NULL;
	struct qitem_t * temp = q->head;

	pthread_mutex_lock(&q->lock);
	proc = q->head->data;
	q->head = q->head->next;
	free(temp);
	if (q->head == NULL) q->tail = NULL;
	pthread_mutex_unlock(&q->lock);

	return proc;
}

/* Put PCB of a process to the queue. */
void en_queue(struct pqueue_t * q, struct pcb_t * proc) {
	// TODO: Update q->tail.
	// Remember to use 'lock' to avoid race condition
	
	// YOUR CODE HERE

	struct qitem_t * proPCB = (struct qitem_t *)malloc(sizeof(struct qitem_t));
	//struct qitem_t * temp = q->head;

	pthread_mutex_lock(&q->lock);
	proPCB->data = proc;
	proPCB->next = NULL;
	if (q->tail == NULL) {
		q->head = proPCB;
		q->tail = q->head;
	}
	else {
		q->tail->next = proPCB;
		q->tail = proPCB;
	}
	/*if (temp == NULL) {
		q->head = proPCB;
		q->tail = q->head;
	}
	else {
		if (proc->priority > temp->data->priority) {
			proPCB->next = temp;
			q->head = proPCB;
		}
		else {
			while (temp->next != NULL) {
				if (proc->priority > temp->next->data->priority) break;
				temp = temp->next;
			}
			proPCB->next = temp->next;
			temp->next = proPCB;
			if (proPCB->next == NULL) q->tail = proPCB;
		}
	}
	*/
	pthread_mutex_unlock(&q->lock);
}

void en_queue_ready(struct pqueue_t * q, struct pcb_t * proc) {

	struct qitem_t * proPCB = (struct qitem_t *)malloc(sizeof(struct qitem_t));
	struct qitem_t * temp = q->head;

	pthread_mutex_lock(&q->lock);
	proPCB->data = proc;
	proPCB->next = NULL;
	if (temp == NULL) {
		q->head = proPCB;
		q->tail = q->head;
	}
	else {
		if (proc->priority > temp->data->priority) {
			proPCB->next = temp;
			q->head = proPCB;
		}
		else {
			while (temp->next != NULL) {
				if (proc->priority > temp->next->data->priority) break;
				temp = temp->next;
			}
			proPCB->next = temp->next;
			temp->next = proPCB;
			if (proPCB->next == NULL) q->tail = proPCB;
		}
	}
	pthread_mutex_unlock(&q->lock);

}
