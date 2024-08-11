#include "dsa.h"

t_Array *new_array(int len) {
	t_Array *new_arr = malloc(sizeof(t_Array));
	if (!new_arr)
		goto error_exit;

	new_arr->len = len;
	new_arr->capacity = len * 2;
	new_arr->content = malloc(new_arr->capacity * sizeof(int));
	if (new_arr->content == NULL)
		goto error_exit;

	return new_arr;

error_exit:
	puts("The array could not be allocated");
	if (new_arr)
		free(new_arr);
	return NULL;
}

void free_array(t_Array *arr) {
	if (!arr)
		return;

	if (arr->content)
		free(arr->content);
	free(arr);
}

void fill_random(t_Array *arr) {
	if (!arr)
		return;
	srand(time(NULL));
	for (int i = 0; i < arr->len; ++i) {
		arr->content[i] = rand();
	}
}

void print_array(t_Array *arr) {
	for (int i = 0; i < arr->len; ++i) {
		printf("%d", arr->content[i]);
		if (i != arr->len - 1)
			printf(", ");
	}
	printf("\n");
}

void dynamic_resize(t_Array *arr) {
	if (arr->len + 1 >= arr->capacity) {
		arr->capacity *= 2;
		int *tmp = realloc(arr->content, arr->capacity * sizeof(int));
		if (!tmp)
			return;
		arr->content = tmp;
	}
}

void insert_at(t_Array *arr, int new_value, int index) {
	if (index >= arr->len || index < 0)
		return;
	dynamic_resize(arr);
	if (!arr->content)
		return;
	for (int i = arr->len; i > index; --i)
		arr->content[i] = arr->content[i - 1];
	arr->content[index] = new_value;
	arr->len++;
}

void append_or_replace(t_Array *arr, int new_value, int index) {
	int idx = (index > arr->len || index < 0) ? arr->len : index;
	if (idx != arr->len) {
		arr->content[idx] = new_value;
		return;
	}
	dynamic_resize(arr);
	if (!arr->content)
		return;
	arr->content[arr->len] = new_value;
	arr->len++;
}

t_Result get_at(t_Array *arr, int idx) {

	t_Result res = {.value = 0, .error = true};
	if (idx >= arr->len || idx < 0)
		return res;
	res.error = false;
	res.value = arr->content[idx];
	return res;
}
