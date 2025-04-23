#ifndef MY_GEAR_H
#define MY_GEAR_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>

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
bool is_file(const char *filepath);
bool is_dir(const char *dirpath);
bool is_dir_empty(const char *dirpath);
// int makedir(const char *dirpath);

// ############################# STRINGS #################################
typedef struct {
  size_t size;
  size_t capacity;
  char **items;
} SplitStrings;

char *trim_leading(char *str);
void trim_trailing(char *str);
void strsplit(SplitStrings *arr, size_t size, char *str, const char *delims);
// TODO: return integer instead of asserting the size
// Example of using `strappend` function:
//
// char buf[1024] = {0};
// strappend(buf, sizeof(buf), "hello ");
// strappend(buf, sizeof(buf), "world!");
// printf("%s\n", buf);
//
// result: hello world!
void strappend(char *buf, size_t size, const char *src);
// TODO: return integer instead of asserting the size
// Example of using `strjoin` function:
//
// char buf[1024] = {0};
// strjoin(buf, sizeof(buf), "/usr", "/");
// strjoin(buf, sizeof(buf), "bin", "/");
// printf("%s\n", buf);
//
// result: /usr/bin
void strjoin(char *buf, size_t size, const char *src, const char *delims);
#define STRCAPITALIZE(str) (str)[0] = toupper((str)[0])
#define STRLOWER(str) for (size_t i = 0; i < strlen(str); i++) str[i] = tolower(str[i])
#define STRLOWER(str) for (size_t i = 0; i < strlen(str); i++) str[i] = toupper(str[i])
// TODO: bool strstartswith(const char *str, const char *suffix);
// TODO: bool strendswith(const char *str, const char *suffix);
// TODO: void strreplace(char *str, size_t size, const char *replacee, const char *replacement);

#endif
