#define _XOPEN_SOURCE 500
#include <assert.h>

#include "../mygear.h"

typedef struct {
  size_t size;
  size_t capacity;
  int   *items;
} StackArray;

void stack(void)
{
  StackArray stack = {0};
  for (int i = 0; i < 128; i++)
    ARRAY_APPEND(&stack, i);

  assert(stack.size == 128);
  assert(stack.capacity == 128);

  for (size_t i = 0; i < stack.size; i++)
    printf("%ld: %d\n", i, stack.items[i]);

  ARRAY_DELETE(&stack);
}

typedef struct {
  size_t size;
  size_t capacity;
  char  **items;
} HeapArray;

void heap(void)
{
  HeapArray heap = {0};
  for (int i = 0; i < 63; i++) {
    char buf[32];
    snprintf(buf, sizeof(buf), "Item %d", i);
    ARRAY_APPEND(&heap, strdup(buf));
  }

  assert(heap.size == 63);
  assert(heap.capacity == 64);

  for (size_t i = 0; i < heap.size; i++)
    printf("%ld: %s\n", i, heap.items[i]);

  ARRAY_DELETE_ALL(&heap);
}

int main(void)
{
  stack();
  heap();
  return 0;
}
