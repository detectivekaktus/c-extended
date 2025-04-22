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
        fprintf(stderr, "Could not allocate memory for array: %s\n", strerror(errno));    \
        exit(1);                                                                          \
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

// ########################### I/O HANDLING ##############################
// Returned pointer is allocated with `malloc`, so you have to call `free`
// on it explicitly.
char *read_entire_file(const char *filename, size_t *size);
// TODO: bool is_file(const char *filepath);
// TODO: bool is_dir(const char *dirpath);
// TODO: bool is_dir_empty(const char *dirpath);
// TODO: int makedir(const char *dirpath);

// ############################# STRINGS #################################
typedef struct {
  size_t size;
  size_t capacity;
  char **items;
} SplitStrings;

char *trim_leading(char *str);
void trim_trailing(char *str);
void strsplit(SplitStrings *arr, size_t size, char *str, const char *delims);
// TODO: void strconcat(const char *dst, const char *src);
// TODO: void strjoin(const char *dst, const char *src, const char *delims);
// TODO: void strcapitalize(char *str);
// TODO: void strlower(char *str);
// TODO: void strupper(char *str);
// TODO: bool strstartswith(const char *str, const char *suffix);
// TODO: bool strendswith(const char *str, const char *suffix);
// TODO: void strreplace(char *str, size_t size, const char *replacee, const char *replacement);

#endif
