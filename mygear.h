#ifndef MY_GEAR_H
#define MY_GEAR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

// Dynamically allocated arrays are defined with the following signature
// typedef struct {
//    size_t size;
//    size_t capacity;
//    void  *items;
// } Array;

#define ARRAY_INIT_CAPACITY 64

#define ARRAY_APPEND(arr, item)                                                           \
  do {                                                                                    \
    if ((arr)->size >= (arr)->capacity) {                                                 \
      (arr)->capacity = (arr)->capacity == 0 ? ARRAY_INIT_CAPACITY : (arr)->capacity * 2; \
      (arr)->items = realloc((arr)->items, sizeof(item) * (arr)->capacity);               \
      if ((arr)->items == NULL) {                                                         \
        fprintf(stderr, "Could not allocate memory for array: %s\n", strerror(errno));      \
      }                                                                                   \
    }                                                                                     \
    (arr)->items[(arr)->size++] = item;                                                   \
  } while (0)

#define ARRAY_DELETE(arr) free((arr)->items)

#define ARRAY_DELETE_ITEMS(arr)               \
  do {                                        \
    for (size_t i = 0; i < (arr)->size; i++)  \
      free((arr)->items[i]);                  \
  } while (0)

#define ARRAY_DELETE_ALL(arr) ARRAY_DELETE_ITEMS(arr); ARRAY_DELETE(arr)


char *read_entire_file(const char *filename, size_t *size);

#endif
