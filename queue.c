#include "dsa.h"
#include <stdlib.h>

t_queue *new_queue() {
	t_queue *Q = malloc(sizeof(t_queue));
	if (!Q)
		return NULL;

	Q->len = 0;
	Q->head = NULL;
	Q->tail = NULL;

	return Q;
}

int enqueue(t_queue *queue, int value) {
	if (!queue)
		return -1;

	t_queue_node *node = malloc(sizeof(t_queue_node));
	if (!node)
		return -1;

	node->value = value;
	node->next = NULL;
	node->prev = queue->tail;

	if (!queue->head) {
		queue->head = node;
	} else {
		queue->tail->next = node;
	}
	queue->tail = node;
	queue->len++;
	return 0;
}

int dequeue(t_queue *queue) {
	if (!queue || queue->len < 1)
		return -1;

	t_queue_node *tmp = queue->head;
	queue->head = tmp->next;
	if (queue->head) {
		queue->head->prev = NULL;
	} else {
		queue->tail = NULL;
	}
	free(tmp);
	queue->len--;
	return 0;
}

void print_queue(t_queue *queue) {
	if (!queue || queue->len < 1)
		return;
	t_queue_node *head = queue->head;
	while (head != NULL) {
		printf("Node value: %d\n", head->value);
		head = head->next;
	}
	printf("Queue len: %d\n", queue->len);
}

void free_q(t_queue *queue) {
	t_queue_node *current = queue->head;
	t_queue_node *next;

	while (current) {
		next = current->next;
		free(current);
		current = next;
	}
	free(queue);
}
