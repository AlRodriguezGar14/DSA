#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

typedef struct {
	int len;
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
