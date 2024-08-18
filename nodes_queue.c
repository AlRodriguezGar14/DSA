#include "dsa.h"
#include <stdlib.h>

t_NQueue *new_nqueue() {
	t_NQueue *Q = malloc(sizeof(t_NQueue));
	if (!Q)
		return NULL;

	Q->len = 0;
	Q->head = NULL;
	Q->tail = NULL;

	return Q;
}

int nenqueue(t_NQueue *queue, t_Node *value) {
	if (!queue)
		return -1;

	t_NQueue_node *node = malloc(sizeof(t_NQueue_node));
	if (!node)
		return -1;

	t_Node *value_copy = malloc(sizeof(t_Node));
	if (!value_copy) {
		free(node);
		return -1;
	}
	value_copy->id = value->id;
	value_copy->left = value->left;
	value_copy->right = value->right;
	node->value = value_copy;
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

int ndequeue(t_NQueue *queue) {
	if (!queue || queue->len < 1)
		return -1;

	t_NQueue_node *tmp = queue->head;
	queue->head = tmp->next;
	if (queue->head) {
		queue->head->prev = NULL;
	} else {
		queue->tail = NULL;
	}
	free(tmp->value);
	free(tmp);
	queue->len--;
	return 0;
}

void print_nqueue(t_NQueue *queue) {
	if (!queue || queue->len < 1)
		return;
	t_NQueue_node *head = queue->head;
	while (head != NULL) {
		printf("Node id: %d\n", head->value->id);
		head = head->next;
	}
	printf("Queue len: %d\n", queue->len);
}

void free_nq(t_NQueue *queue) {
	t_NQueue_node *current = queue->head;
	t_NQueue_node *next;

	while (current) {
		next = current->next;
		free(current->value);
		free(current);
		current = next;
	}
	free(queue);
}
