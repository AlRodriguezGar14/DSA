#include "dsa.h"

static void copy_array(int *src, int *dest, int stop_at, int copy_from) {
	for (int i = 0; i < stop_at; ++i) {
		dest[i] = src[copy_from + i];
	}
}

static void merge(int *arr, int start, int middle, int end) {

	int len_left = middle - start + 1;
	int len_right = end - middle;

	int left[len_left], right[len_right];
	copy_array(arr, left, len_left, start);
	copy_array(arr, right, len_right, middle + 1);

	// Sort logic
	int i = 0, j = 0;
	for (int k = start; k <= end; ++k) {

		if (i < len_left && (j >= len_right || left[i] <= right[j]))
			arr[k] = left[i++];
		else
			arr[k] = right[j++];
	}
}

void merge_sort(int *arr, int start, int end) {
	if (start >= end)
		return;

	// end - start = length
	// start + <- avoids potential segfaults for negative values
	int middle = start + (end - start) / 2;
	merge_sort(arr, start, middle);
	merge_sort(arr, middle + 1, end);
	merge(arr, start, middle, end);
}
