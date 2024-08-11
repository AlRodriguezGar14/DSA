#include "dsa.h"

void test_array() {
  t_Array *arr = new_array(10);
  t_Result res;
  fill_random(arr);
  print_array(arr);
  append_or_replace(arr, 42, -1);
  append_or_replace(arr, 19, 5);
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
