#include "dsa.h"

// End is not the length of the array but the last accessible value: len - 1
void quicksort(int *arr, int start, int end) {
	if (start >= end)
		return;

	int pivot = arr[end];
	int idx = start;

	for (int i = start; i <= end; ++i) {
		if (arr[i] < pivot) {
			int tmp = arr[i];
			arr[i] = arr[idx];
			arr[idx] = tmp;
			idx++;
		}
	}

	int tmp = arr[idx];
	arr[idx] = pivot;
	arr[end] = tmp;

	quicksort(arr, start, idx - 1);
	quicksort(arr, idx + 1, end);
}
