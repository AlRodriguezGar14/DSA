#include "dsa.h"

int main() {
	t_Array *arr = new_array(10);
  t_Result res;
	fill_random(arr);
	print_array(arr);
	append_or_replace(arr, 42, -1);
	append_or_replace(arr, 19, 5);
	print_array(arr);
	insert_at(arr, 1771, 0);
	print_array(arr);
  res = get_at(arr, 5);
  if (!res.error)
    printf("The value at 5 is: %d\n", res.value);
  res = get_at(arr, 50);
  if (res.error)
    fprintf(stderr, "The index is out of bounds\n");
	free_array(arr);
	return 0;
}
