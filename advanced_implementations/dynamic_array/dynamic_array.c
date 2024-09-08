#include <stdio.h>
#include <stdlib.h>

#define da_append(arr, item)                                                   \
  do                                                                           \
  {                                                                            \
    if (arr.count >= arr.capacity)                                             \
    {                                                                          \
      if (arr.capacity == 0)                                                   \
        arr.capacity = 128;                                                    \
      else                                                                     \
        arr.capacity *= 2;                                                     \
                                                                               \
      arr.items = realloc (arr.items, arr.capacity * sizeof (arr.items));      \
    }                                                                          \
                                                                               \
    arr.items[arr.count++] = item;                                             \
  } while (0)

typedef struct
{
  int *items;
  size_t count;
  size_t capacity;
} Numbers;

typedef struct
{
  float *items;
  size_t count;
  size_t capacity;
} Floats;

typedef struct
{
  char **items;
  size_t count;
  size_t capacity;
} String;

int
main ()
{
  Numbers arr = {.items = NULL, .count = 0, .capacity = 0};
  Floats arr_float = {.items = NULL, .count = 0, .capacity = 0};
  String str = {.items = NULL, .count = 0, .capacity = 0};

  da_append (arr, 1);
  da_append (arr_float, 42.0f);
  da_append (str, "hello");
  da_append (str, "world");
  printf ("%d\n", arr.items[0]);
  printf ("%f\n", arr_float.items[0]);
  printf ("%s\n", str.items[0]);
  printf ("%s\n", str.items[1]);
  free (arr.items);
  free (arr_float.items);
  free (str.items);
  return 0;
}
