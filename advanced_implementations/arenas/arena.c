#include <stdalign.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define REGION_DEFAULT_CAPACITY 4 * 1024 // 4KB
//
/*
 * Documentation:
 * > Understanding and using c pointers
 * > Computer systems: a programmer's perspective chapters 6 and 9 (especially)
 * > https://embeddedartistry.com/blog/2017/02/22/generating-aligned-memory/
 * > https://www.codeproject.com/Articles/1123638/MASM-Stack-Memory-Alignment
 * >
 * https://www.gingerbill.org/article/2019/02/08/memory-allocation-strategies-002/
 * >
 * https://blog.andreiavram.ro/memory-alignment-undefined-behavior/#:~:text=Memory%20alignment%20is%20important%20for,and%20some%20padding%20is%20added.
 * */

/* The Arena is built upon one or multiple regions */
typedef struct Region Region;
struct Region
{
  Region *next;    // next region, created by the arena if this one is full
  size_t count;    // number of bits used
  size_t capacity; // total number of bits available
  size_t allocated_bytes;
  uint8_t data[]; // array of memory
};

typedef struct
{
  Region *begin, *end;
  int regions;
  size_t memory_allocated;
  pthread_mutex_t mutex;
} Arena;

/* new arena */
Region *
new_region (size_t capacity)
{
  size_t size_bytes = sizeof (Region) + sizeof (uint8_t) * capacity;
  Region *region
    = (Region *) malloc (size_bytes); // Allocates the size of region and the
                                      // capacity needed for data[]
  if (region == NULL)
  {
    fprintf (stderr, "Failed to allocate memory for region\n");
    return NULL;
  }
  region->next = NULL;
  region->count = 0;
  region->capacity = capacity;
  region->allocated_bytes = size_bytes;
  return region;
}

int
init_arena (Arena *a)
{
  a->begin = NULL;
  a->end = NULL;
  a->regions = 0;
  a->memory_allocated = 0;
  if (pthread_mutex_init (&a->mutex, NULL) != 0)
  {
    fprintf (stderr, "Failed to initialize the mutex\n");
    return -1;
  }
  return 0;
}

/* arena alloc */
void *
arena_alloc (Arena *a, size_t size_bytes)
{
  if (size_bytes == 0 || !a)
    return NULL;

  pthread_mutex_lock (&a->mutex);

  /*
   * Allign the number of bytes I want to allocate; rounded to the nearest
   * multiple of max_align_t, which guarantees it can cover any data size
   * (including doubles)
   *
   * imagine that alignof(max_align_t) is 16 bytes: 00010000
   * alignof(max_align_t) - 1 is 15: 00001111
   *
   * them mask of the operator NOT zeroes the lower bits ~alignof(max_align_t) -
   * 1 is ~15: 11110000
   * AND (&) aligns the size + alignof(max_align_t) - 1 to the nearest multiple
   * of alignment. For example: 0, 16, 32, 48... etc.
   *
   * If I want to add for example 30 bits, the size is adjusted to 32
   * the -1 is very important to set the aproximation to the lowest boundary (32
   * instead of 48 for example)
   * */
  size_t size
    = (size_bytes + alignof (max_align_t) - 1) & ~(alignof (max_align_t) - 1);

  if (a->end == NULL)
  {
    size_t capacity = REGION_DEFAULT_CAPACITY;
    if (capacity < size)
      capacity = size;
    a->end = new_region (capacity);
    if (a->end == NULL)
    {
      pthread_mutex_unlock (&a->mutex);
      return NULL;
    }
    a->regions++;
    a->memory_allocated += a->end->allocated_bytes;
    a->begin = a->end;
  }

  // If the current region has no enough space, advance to the next region
  while (a->end->count + size > a->end->capacity && a->end->next != NULL)
    a->end = a->end->next;

  // If no space is found, create a new region;
  if (a->end->count + size > a->end->capacity)
  {
    size_t capacity = REGION_DEFAULT_CAPACITY;
    if (capacity < size)
      capacity = size;
    a->end->next = new_region (capacity);
    if (a->end == NULL)
    {
      pthread_mutex_unlock (&a->mutex);
      return NULL;
    }
    a->regions++;
    a->memory_allocated += a->end->next->allocated_bytes;
    a->end = a->end->next;
  }
  // Point to the first available data in the region
  void *result = &a->end->data[a->end->count];
  memset (result, 0, size);
  a->end->count += size;
  pthread_mutex_unlock (&a->mutex);
  return result;
}

void *
arena_realloc (Arena *a, void *ptr, size_t old_size, size_t new_size)
{
  if (new_size == 0)
  {
    fprintf (stderr, "Invalid use of arena_realloc\n");
    return NULL;
  }
  if (!ptr)
    return arena_alloc (a, new_size);

  pthread_mutex_lock (&a->mutex);
  size_t old_size_aligned
    = (old_size + alignof (max_align_t) - 1) & ~(alignof (max_align_t) - 1);
  size_t new_size_aligned
    = (new_size + alignof (max_align_t) - 1) & ~(alignof (max_align_t) - 1);

  // If the pointer is at the end of the current region and there's enough room,
  // we can just expand
  if ((uint8_t *) ptr + old_size_aligned
        == (uint8_t *) &a->end
             ->data[a->end->count] // check if the old size matches with the
                                   // last region count update (this means
                                   // there's contigous space that has not been
                                   // used)
      && a->end->count + (new_size_aligned - old_size_aligned)
           <= a->end->capacity)
  {
    a->end->count += new_size_aligned - old_size_aligned;

    pthread_mutex_unlock (&a->mutex);
    return ptr;
  }

  pthread_mutex_unlock (&a->mutex);
  // Allocate new memory
  void *new_ptr = arena_alloc (a, new_size);
  if (!new_ptr)
    return NULL;

  // Copy the smaller of old_size and new_size to avoid memory corruption
  size_t copy_size = old_size < new_size ? old_size : new_size;
  memcpy (new_ptr, ptr, copy_size);
  return new_ptr;
}

/* arena free */
// Set everything to 0 so that the memory can be overwritten. This way no new
// mallocs are needed to reuse the arena
void
reset_arena (Arena *a)
{
  pthread_mutex_lock (&a->mutex);
  for (Region *r = a->begin; r != NULL; r = r->next)
    r->count = 0;
  a->end = a->begin;
  pthread_mutex_unlock (&a->mutex);
}

void
free_arena (Arena *a)
{
  pthread_mutex_lock (&a->mutex);
  Region *r = a->begin;

  while (r)
  {
    Region *r0 = r;
    r = r->next;
    free (r0);
  }
  a->begin = NULL;
  a->end = NULL;
  pthread_mutex_unlock (&a->mutex);
  pthread_mutex_destroy (&a->mutex);
}

/* Testing it works */
typedef struct
{
  char *string;
} AnnoyingStruct;

int
main (void)
{
  Arena basic_arena;

  if (init_arena (&basic_arena) != 0)
    return 1;

  AnnoyingStruct *as = arena_alloc (&basic_arena, sizeof (AnnoyingStruct));
  printf ("The mem pointer after the first alloc (initialization): %zu\n",
          basic_arena.end->count);
  as->string = arena_alloc (&basic_arena, 1000);
  printf ("The mem pointer after alloc of 1000: %zu\n", basic_arena.end->count);
  strcpy (as->string, "This is just a random string");
  as->string[strlen ("This is just a random string")] = '\0';
  printf ("%s\n", as->string);

  size_t prev = basic_arena.end->count;
  printf ("The mem pointer before allocating 1 byte in string: %zu\n", prev);
  char *string = arena_alloc (&basic_arena, 1);
  printf ("The mem pointer after allocating 1 byte in string (even if 1 byte, "
          "the count should have advanced 16 bytes): %zu\n\tResult: %zu\n",
          basic_arena.end->count, basic_arena.end->count - prev);
  prev = basic_arena.end->count;
  string
    = arena_realloc (&basic_arena, &string, 1, strlen ("Hello World123456"));
  printf ("The mem pointer after reallocating strlen(HelloWorld123456) byte in "
          "string (it should grow by 32 bytes as the string is longer than "
          "16): %zu\n\tResult: %zu\n",
          basic_arena.end->count, basic_arena.end->count - prev);

  strcpy (string, "Hello World123456");
  printf ("%s\n", string);

  printf ("Allocated regions: %d\n", basic_arena.regions);
  printf ("Allocated memory: %zu\n", basic_arena.memory_allocated);
  free_arena (&basic_arena);

  return 0;
}
