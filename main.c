#include "dsa.h"

int main() {
	t_Array *arr = new_array(10);
	fill_random(arr);
	print_array(arr);
	append_or_replace(arr, 42, -1);
	append_or_replace(arr, 19, 5);
	print_array(arr);
	insert_at(arr, 1771, 0);
	print_array(arr);
	free_array(arr);
	return 0;
}
