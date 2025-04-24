#define _XOPEN_SOURCE 500
#include <assert.h>

#include "../gear.h"

typedef struct {
  unsigned char r;
  unsigned char g;
  unsigned char b;
  unsigned char a;
} Color;

typedef struct {
  size_t size;
  size_t capacity;
  Color *items;
} ColorArray;

typedef struct {
  size_t size;
  size_t capacity;
  int   *items;
} IntArray;

typedef struct {
  size_t size;
  size_t capacity;
  char  **items;
} StrArray;

void stack(void)
{
  IntArray stack = {0};
  for (int i = 0; i < 128; i++)
    ARRAY_APPEND(&stack, i);
  ARRAY_PREPEND(&stack, 69);
  ARRAY_PREPEND(&stack, 420);

  ARRAY_REMOVE(&stack, 0);
  ARRAY_REMOVE(&stack, 0);

  FOREACH(int, num, &stack)
    printf("%d\n", *num);

  assert(stack.size == 128);
  assert(stack.capacity == 256);

  ARRAY_DELETE(&stack);
}

void heap(void)
{
  StrArray heap = {0};
  for (int i = 0; i < 63; i++) {
    char buf[32];
    snprintf(buf, sizeof(buf), "Item %d", i);
    ARRAY_APPEND(&heap, strdup(buf));
  }

  assert(heap.size == 63);
  assert(heap.capacity == 64);

  ARRAY_DELETE_ALL(&heap);
}

void foreaching(void)
{
  IntArray ints = {0};
  for (int i = 0; i < 10; i++)
    ARRAY_APPEND(&ints, i);
  FOREACH(int, num, &ints)
    printf("%d\n", *num);
  ARRAY_DELETE(&ints);

  StrArray strs = {0};
  for (int i = 0; i < 10; i++) {
    char buf[32];
    snprintf(buf, sizeof(buf), "Item %d", i);
    ARRAY_APPEND(&strs, strdup(buf));
  }
  FOREACH(char *, str, &strs)
    printf("%s\n", *str);
  ARRAY_DELETE_ALL(&strs);

  ColorArray colors = {0};
  for (int i = 0; i < 10; i++) {
    Color c = { .r = i, .g = i, .b = i, .a = 255};
    ARRAY_APPEND(&colors, c);
  }
  FOREACH(Color, c, &colors)
    printf("%d %d %d\n", c->r, c->g, c->b);
  ARRAY_DELETE(&colors);
}

int main(void)
{
  stack();
  heap();
  foreaching();
  return 0;
}
