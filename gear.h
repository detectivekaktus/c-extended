// TODO: prepend all function and macro names with GEAR_ and gear_
// for consistency and "namespacing" the functions from other libraries
// that may be in conflict.
#ifndef GEAR_H
#define GEAR_H

#include <assert.h>
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
#define ARRAY_GROW(arr, item)                                                             \
  do {                                                                                    \
    if ((arr)->size >= (arr)->capacity) {                                                 \
      (arr)->capacity = (arr)->capacity == 0 ? ARRAY_INIT_CAPACITY : (arr)->capacity * 2; \
      (arr)->items = realloc((arr)->items, sizeof(item) * (arr)->capacity);               \
      if ((arr)->items == NULL) {                                                         \
        fprintf(stderr, "Could not allocate memory for array: %s\n", strerror(errno));    \
        exit(1);                                                                          \
      }                                                                                   \
    }                                                                                     \
  } while (0)

#define ARRAY_APPEND(arr, item)         \
  do {                                  \
    ARRAY_GROW(arr, item);              \
    (arr)->items[(arr)->size++] = item; \
  } while (0)

#define ARRAY_PREPEND(arr, item)                                            \
  do {                                                                      \
    ARRAY_GROW(arr, item);                                                  \
    if ((arr)->size == 0)                                                   \
      ARRAY_APPEND(arr, item);                                              \
    else {                                                                  \
      memmove((arr)->items + 1, (arr)->items, sizeof(item) * (arr)->size);  \
      (arr)->items[0] = item;                                               \
      (arr)->size++;                                                        \
    }                                                                       \
  } while (0)

#define ARRAY_REMOVE(arr, index)                                    \
  do {                                                              \
    assert((long long int)index >= 0 && (arr)->size > 0);           \
    if ((size_t)index != (arr)->size - 1)                           \
      memmove((arr)->items + index,                                 \
              (arr)->items + index + 1,                             \
              sizeof((arr)->items[0]) * ((arr)->size - index - 1)); \
    (arr)->size--;                                                  \
  } while (0)

#define ARRAY_REMOVE_BACK(arr) ARRAY_REMOVE(arr, (arr)->size - 1)
#define ARRAY_REMOVE_FRONT(arr) ARRAY_REMOVE(arr, 0)
#define ARRAY_POP_BACK(arr, var)          \
  do {                                    \
    var = (arr)->items[(arr)->size - 1];  \
    ARRAY_REMOVE_BACK(arr);               \
  } while (0)

#define ARRAY_POP_FRONT(arr, var) \
  do {                            \
    var = (arr)->items[0];        \
    ARRAY_REMOVE_FRONT(arr);      \
  } while (0)

#define ARRAY_DELETE(arr) free((arr)->items)
#define ARRAY_DELETE_ITEMS(arr) for (size_t i = 0; i < (arr)->size; i++) free((arr)->items[i]);
#define ARRAY_DELETE_ALL(arr) ARRAY_DELETE_ITEMS(arr); ARRAY_DELETE(arr)

// Example of using FOREACH on an array
//
// typedef struct {
//    size_t size;
//    size_t capacity;
//    int   *items;
// } IntArray;
//
// IntArray arr = {0};
// for (int i = 0; i < 10; i++)
//    ARRAY_APPEND(&arr, i);
//
// FOREACH(int, num, &arr)
//    printf("%d", *num);
#define FOREACH(type, item, arr) for (type *item = (arr)->items; item != (arr)->items + (arr)->size; item++)

// ########################### I/O HANDLING ##############################
#define GEAR_MAX_DIRNAME_LENGTH 512 
#define GEAR_MAX_DIR_CHILDREN   64
// Returned pointer is allocated with `malloc`, so you have to call `free`
// on it explicitly.
char *read_entire_file(const char *filename, size_t *size);
bool is_file(const char *filepath);
bool is_dir(const char *dirpath);
bool is_dir_empty(const char *dirpath);
int makedir(char *dirpath);

// ############################# STRINGS #################################
typedef struct {
  size_t size;
  size_t capacity;
  char **items;
} SplitStrings;

char *trim_leading(char *str);
void trim_trailing(char *str);
void strsplit(SplitStrings *arr, size_t size, char *str, const char *delims);
// Example of using `strappend` function:
//
// char buf[1024] = {0};
// strappend(buf, sizeof(buf), "hello ");
// strappend(buf, sizeof(buf), "world!");
// printf("%s\n", buf);
//
// result: hello world!
void strappend(char *buf, size_t size, const char *src);
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
#define STRLOWER(str) for (size_t i = 0; i < strlen(str); i++) (str)[i] = tolower((str)[i])
#define STRUPPER(str) for (size_t i = 0; i < strlen(str); i++) (str)[i] = toupper((str)[i])
bool strstartswith(const char *str, const char *prefix);
bool strendswith(const char *str, const char *suffix);
void strreplace(char *buf, size_t size, size_t memsize, char *str, const char *replacee, const char *replacement);

// ############################# UTILS #################################

#define GEAR_NEW(_type, n) malloc(sizeof(_type) * n)
#define GEAR_NEW_CLEAN(_type, n) calloc(n, sizeof(_type))

// ############################# HASHMAP #################################
typedef enum {
  MAP_TYPE_STRING,
  MAP_TYPE_INTEGER,
  MAP_TYPE_LONG,
  MAP_TYPE_DOUBLE
} GearMapType;

typedef struct {
  void *key;
  void *value;
// TODO: void *next;
} GearMapSlot;

typedef struct {
  GearMapType key_type;
  size_t      size;
  size_t      capacity;
  GearMapSlot **slots;
} GearMap;

GearMap *map_init(const GearMapType key_type, const size_t capacity);
void map_insert(GearMap *map, void *key, void *value);
int map_remove(const GearMap *map, void *key);
void *map_find(const GearMap *map, void *key);
#define MAP_EXISTS(map, key) map_find(map, key) != NULL
void map_delete_kvpairs(GearMap *map);
void map_delete_keys(GearMap *map);
void map_delete_values(GearMap *map);
void map_delete_slots(GearMap *map);
#define MAP_DELETE(map) \
  do {                  \
    free(map->slots); \
    free(map);          \
  } while (0)

// TODO: macro for getting the value by key without weird syntax

#endif
