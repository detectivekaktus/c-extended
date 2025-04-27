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
    GEAR_ARRAY_APPEND(&stack, i);
  GEAR_ARRAY_PREPEND(&stack, 69);
  GEAR_ARRAY_PREPEND(&stack, 420);

  GEAR_ARRAY_REMOVE(&stack, 0);
  GEAR_ARRAY_REMOVE(&stack, 0);

  GEAR_ARRAY_REMOVE_FRONT(&stack);
  GEAR_ARRAY_REMOVE_BACK(&stack);
  int a,b;
  GEAR_ARRAY_POP_FRONT(&stack, a);
  GEAR_ARRAY_POP_BACK(&stack, b);
  (void)a;
  (void)b;

  GEAR_FOREACH(int, num, &stack)
    printf("%d\n", *num);

  assert(stack.size == 124);
  assert(stack.capacity == 256);

  GEAR_ARRAY_DELETE(&stack);
}

void heap(void)
{
  StrArray heap = {0};
  for (int i = 0; i < 63; i++) {
    char buf[32];
    snprintf(buf, sizeof(buf), "Item %d", i);
    GEAR_ARRAY_APPEND(&heap, strdup(buf));
  }

  assert(heap.size == 63);
  assert(heap.capacity == 64);

  GEAR_ARRAY_DELETE_ALL(&heap);
}

void foreaching(void)
{
  IntArray ints = {0};
  for (int i = 0; i < 10; i++)
    GEAR_ARRAY_APPEND(&ints, i);
  GEAR_FOREACH(int, num, &ints)
    printf("%d\n", GEAR_FOREACH_GET(num));
  GEAR_ARRAY_DELETE(&ints);

  StrArray strs = {0};
  for (int i = 0; i < 10; i++) {
    char buf[32];
    snprintf(buf, sizeof(buf), "Item %d", i);
    GEAR_ARRAY_APPEND(&strs, strdup(buf));
  }
  GEAR_FOREACH(char *, str, &strs)
    printf("%s\n", GEAR_FOREACH_GET(str));
  GEAR_ARRAY_DELETE_ALL(&strs);

  ColorArray colors = {0};
  for (int i = 0; i < 10; i++) {
    Color c = { .r = i, .g = i, .b = i, .a = 255};
    GEAR_ARRAY_APPEND(&colors, c);
  }
  GEAR_FOREACH(Color, c, &colors)
    printf("%d %d %d\n", c->r, c->g, c->b);
  GEAR_ARRAY_DELETE(&colors);
}

int main(void)
{
  stack();
  heap();
  foreaching();
  return 0;
}
