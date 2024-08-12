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

void test_merge_sort() {
	srand(time(0));

	int arr[50];
	for (int i = 0; i < 50; ++i) {
		arr[i] = rand() % 100;
	}

	puts("before merge_sort():");
	for (int i = 0; i < 50; ++i) {
		printf("%d", arr[i]);
		if (i < 49)
			printf(" ");
	}
	printf("\n");

	// end is not the length but the last index
	// merge_sort(arr, 0, 49);
	merge_sort(arr, 0, sizeof(arr) / sizeof(int) - 1);

	puts("after merge_sort():");
	for (int i = 0; i < 50; ++i) {
		printf("%d", arr[i]);
		if (i < 49)
			printf(" ");
	}
	printf("\n");
}
