#include "dsa.h"

t_Array *new_array(int len) {
	t_Array *new_arr = malloc(sizeof(t_Array));
	if (!new_arr) goto error_exit;

	new_arr->len = len;
	new_arr->content = malloc(len * sizeof(int));
	if (new_arr->content == NULL) goto error_exit;

	return new_arr;

	error_exit:
		puts("The array could not be allocated");
		if (new_arr) free(new_arr);
		return NULL;
}

void free_array(t_Array *arr) {
	if (!arr) return ;

	if (arr->content) free(arr->content);
	free(arr);
}

void fill_random(t_Array *arr) {
	if (!arr) return ;
	srand(time(NULL));
	for (int i = 0; i < arr->len; ++i) {
		arr->content[i] = rand();
	}
}

void print_array(t_Array *arr) {
	for (int i = 0; i < arr->len; ++i) {
		printf("%d", arr->content[i]);
		if (i != arr->len - 1) printf(", ");
	}
	printf("\n");
}

void insert_at(t_Array *arr, int new_value, int index) {
	if (index >= arr->len || index < 0) return ;

	t_Array *new_arr = new_array(arr->len + 1);
	if (!new_arr) return ;
	// for (int i = 0; i < index; ++i) new_arr->content[i] = arr->content[i];
	memmove(new_arr->content, arr->content, (index) * sizeof(int));
	new_arr->content[index] = new_value;
	for (int i = index; i < arr->len; ++i) new_arr->content[i + 1] = arr->content[i]; 
	free(arr->content);
	arr->content = new_arr->content;
	arr->len = new_arr->len;
	free(new_arr);
}

void append_or_replace(t_Array *arr, int new_value, int index) {
	int idx = (index > arr->len || index < 0) ? arr->len : index;

	if (idx != arr->len)  {
		arr->content[idx] = new_value;
		return;
	}
	t_Array *new_arr = new_array(arr->len + 1);
	if (!new_arr) return ;
	memmove(new_arr->content, arr->content, arr->len * sizeof(int));
	new_arr->content[idx] = new_value;
	free(arr->content);
	arr->content = new_arr->content;
	arr->len = new_arr->len;
	free(new_arr);
}