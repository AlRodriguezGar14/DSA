#include "dsa.h"

int binary_search(int *arr, int len, int target) {
	int middle, check, left = 0, right = len;

	while (left < right) {
		middle = left + (right - left) / 2;
		check = arr[middle];
		if (check == target)
			return middle;
		else if (check < target)
			left = middle + 1;
		else
			right = middle - 1;
	}

	return -1;
}
