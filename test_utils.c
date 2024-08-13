#include "dsa.h"

void test_array() {
	/* t_Array *arr = new_array(10); */
	t_Array *arr = new_array(2);
	t_Result res;
	fill_random(arr);
	print_array(arr);
	append_or_replace(arr, 42, -1);
	append_or_replace(arr, 19, 0);
	print_array(arr);
	insert_at(arr, 1771, 0);
	print_array(arr);
	res = get_at(arr, arr->len - 1);
	if (!res.error)
		printf("The value at %d is: %d\n", arr->len - 1, res.value);
	res = get_at(arr, arr->len);
	if (res.error)
		fprintf(stderr, "The index %d is out of bounds\n", arr->len);
	free_array(arr);
}

static void build_random_arr(int *arr, int len) {
	for (int i = 0; i < len; ++i)
		arr[i] = rand() % 100;
}

static void print_arr(int *arr, int len) {
	for (int i = 0; i < 50; ++i) {
		printf("%d", arr[i]);
		if (i < 49)
			printf(" ");
	}
	printf("\n");
}

void test_merge_sort() {
	int arr[50];

	build_random_arr(arr, 50);

	puts("before merge_sort():");
	print_arr(arr, 50);

	// end is not the length but the last index
	// merge_sort(arr, 0, 49);
	merge_sort(arr, 0, sizeof(arr) / sizeof(int) - 1);

	puts("after merge_sort():");
	print_arr(arr, 50);
}

void test_quicksort() {
	int arr[50];

	build_random_arr(arr, 50);

	puts("before quicksort():");
	print_arr(arr, 50);

	// end is not the length but the last index
	// merge_sort(arr, 0, 49);
	quicksort(arr, 0, sizeof(arr) / sizeof(int) - 1);

	puts("after quicksort():");
	print_arr(arr, 50);
}

// Return the find index, not the value
void test_binary_search() {
	int arr[50];

	build_random_arr(arr, 50);
	print_arr(arr, 50);
	quicksort(arr, 0, sizeof(arr) / sizeof(int) - 1);
	print_arr(arr, 50);
	int val = arr[0];
	int val2 = arr[47];
	printf("Search for a non existent value 4242... return index: %d\n",
		   binary_search(arr, 50, 4242));
	printf("Search for existent value %d... return index: %d\n", val,
		   binary_search(arr, 50, val));
	printf("Search for existent value %d... return index: %d\n", val2,
		   binary_search(arr, 50, val2));

	printf("\n");
}
