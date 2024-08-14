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

// merge sort
void merge_sort(int *arr, int start, int end);

// quicksort
void quicksort(int *arr, int start, int end);

// binary search
int binary_search(int *arr, int len, int target);

// binary search tree
//
typedef struct s_Node {

	int id;
	struct s_Node *left;
	struct s_Node *right;
} t_Node;

// Test utils
void test_array();
void test_merge_sort();
void test_quicksort();
void test_binary_search();
void test_bst();
