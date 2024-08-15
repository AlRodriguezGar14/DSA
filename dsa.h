#pragma once

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Array
typedef struct {
	int len;
	int capacity;
	int *content;
} t_Array;

typedef struct {
	int value;
	bool error;
} t_Result;

t_Array *new_array(int len);
void free_array(t_Array *arr);
void fill_random(t_Array *arr);
void print_array(t_Array *arr);
void append_or_replace(t_Array *arr, int new_value, int index);
void insert_at(t_Array *arr, int new_value, int index);
t_Result get_at(t_Array *arr, int idx);

// Queue
typedef struct s_queue_node {
	int value;
	struct s_queue_node *next;
	struct s_queue_node *prev;
} t_queue_node;

typedef struct s_queue {
	t_queue_node *head;
	t_queue_node *tail;
	int len;
} t_queue;

t_queue *new_queue();
int enqueue(t_queue *queue, int value);
int dequeue(t_queue *queue);
void print_queue(t_queue *queue);
void free_q(t_queue *queue);

// merge sort
void merge_sort(int *arr, int start, int end);

// quicksort
void quicksort(int *arr, int start, int end);

// binary search
int binary_search(int *arr, int len, int target);

// binary search tree
typedef struct s_Node {

	int id;
	struct s_Node *left;
	struct s_Node *right;
} t_Node;

t_Node *new_node(int id);
t_Node *bst_insert(t_Node *root, int id);
bool bst_find(t_Node *root, int id);
t_Node *bst_get(t_Node *root, int id);
int bst_smallest_value(t_Node *root);
int bst_largest_value(t_Node *root);
t_Node *bst_smallest_node(t_Node *root);
t_Node *bst_remove_node(t_Node *root, int id);
void free_tree(t_Node *root);
void print_tree(t_Node *root, int space);
void bst_traverse(t_Node *root); // depth first search

// Test utils
void test_array();
void test_merge_sort();
void test_quicksort();
void test_binary_search();
void test_bst();
void test_bst();
void test_queue();
