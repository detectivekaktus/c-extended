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

#define GEAR_ARRAY_INIT_CAPACITY 64
#define GEAR_ARRAY_GROW(arr, item)                                                              \
  do {                                                                                          \
    if ((arr)->size >= (arr)->capacity) {                                                       \
      (arr)->capacity = (arr)->capacity == 0 ? GEAR_ARRAY_INIT_CAPACITY : (arr)->capacity * 2;  \
      (arr)->items = realloc((arr)->items, sizeof(item) * (arr)->capacity);                     \
      if ((arr)->items == NULL) {                                                               \
        fprintf(stderr, "Could not allocate memory for array: %s\n", strerror(errno));          \
        exit(1);                                                                                \
      }                                                                                         \
    }                                                                                           \
  } while (0)

#define GEAR_ARRAY_APPEND(arr, item)    \
  do {                                  \
    GEAR_ARRAY_GROW(arr, item);         \
    (arr)->items[(arr)->size++] = item; \
  } while (0)

#define GEAR_ARRAY_PREPEND(arr, item)                                       \
  do {                                                                      \
    GEAR_ARRAY_GROW(arr, item);                                             \
    if ((arr)->size == 0)                                                   \
      GEAR_ARRAY_APPEND(arr, item);                                         \
    else {                                                                  \
      memmove((arr)->items + 1, (arr)->items, sizeof(item) * (arr)->size);  \
      (arr)->items[0] = item;                                               \
      (arr)->size++;                                                        \
    }                                                                       \
  } while (0)

#define GEAR_ARRAY_REMOVE(arr, index)                               \
  do {                                                              \
    assert((long long int)index >= 0 && (arr)->size > 0);           \
    if ((size_t)index != (arr)->size - 1)                           \
      memmove((arr)->items + index,                                 \
              (arr)->items + index + 1,                             \
              sizeof((arr)->items[0]) * ((arr)->size - index - 1)); \
    (arr)->size--;                                                  \
  } while (0)

#define GEAR_ARRAY_REMOVE_BACK(arr) GEAR_ARRAY_REMOVE(arr, (arr)->size - 1)
#define GEAR_ARRAY_REMOVE_FRONT(arr) GEAR_ARRAY_REMOVE(arr, 0)
#define GEAR_ARRAY_POP_BACK(arr, var)     \
  do {                                    \
    var = (arr)->items[(arr)->size - 1];  \
    GEAR_ARRAY_REMOVE_BACK(arr);          \
  } while (0)

#define GEAR_ARRAY_POP_FRONT(arr, var)  \
  do {                                  \
    var = (arr)->items[0];              \
    GEAR_ARRAY_REMOVE_FRONT(arr);       \
  } while (0)

#define GEAR_ARRAY_DELETE(arr) free((arr)->items)
#define GEAR_ARRAY_DELETE_ITEMS(arr) for (size_t i = 0; i < (arr)->size; i++) free((arr)->items[i]);
#define GEAR_ARRAY_DELETE_ALL(arr) GEAR_ARRAY_DELETE_ITEMS(arr); GEAR_ARRAY_DELETE(arr)

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
//    GEAR_ARRAY_APPEND(&arr, i);
//
// GEAR_FOREACH(int, num, &arr)
//    printf("%d", GEAR_FOREACH_GET(num));
#define GEAR_FOREACH(_type, _item, arr) for (_type *_item = (arr)->items; _item != (arr)->items + (arr)->size; _item++)
#define GEAR_FOREACH_GET(_item) *_item

// ########################### I/O HANDLING ##############################
#define GEAR_MAX_DIRNAME_LENGTH 512 
#define GEAR_MAX_DIR_CHILDREN   64
// Returned pointer is allocated with `malloc`, so you have to call `free`
// on it explicitly.
char *gear_read_entire_file(const char *filename, size_t *size);
bool gear_is_file(const char *filepath);
bool gear_is_dir(const char *dirpath);
bool gear_is_dir_empty(const char *dirpath);
// This function also creates subdirectories if any is missing.
int gear_makedir(char *dirpath);

// ############################# STRINGS #################################
typedef struct {
  size_t size;
  size_t capacity;
  char **items;
} GearSplitStrings;

char *gear_trim_leading(char *str);
void gear_trim_trailing(char *str);
// void gear_strsplit(GearSplitStrings *arr, size_t msize, char *str, const char *delims):
//    arr     - array to store substrings
//    msize   - max size of substring included the NULL terminator
//    str     - string to split
//    delims  - delimiters
//
// Example of using `gear_strsplit` function:
//
// char str[64] = "string to split in substrings"
// GearSplitStrings arr = {0};
// gear_strsplit(&arr, sizeof(str), str, " ");
// GEAR_FOREACH(char *, s, &arr)
//   printf("%s\n", GEAR_FOREACH_GET(s));
// GEAR_ARRAY_DELETE_ALL(&arr);
//  
// result: string
//         to
//         split
//         in
//         substrings
void gear_strsplit(GearSplitStrings *arr, size_t msize, char *str, const char *delims);
// void gear_strappend(char *buf, size_t size, const char *src):
//    buf     - buffer where the final string will be stored
//              included the NULL terminator. Newly created
//              buffer must be all initialized to 0.
//    size  - size of buffer `buf`
//    src   - string to append
//
// Example of using `gear_strappend` function:
//
// char buf[1024] = {0};
// gear_strappend(buf, sizeof(buf), "hello ");
// gear_strappend(buf, sizeof(buf), "world!");
// printf("%s\n", buf);
//
// result: hello world!
void gear_strappend(char *buf, size_t size, const char *src);
// void gear_strjoin(char *buf, size_t size, const char *src, const char *delims):
//    buf     - buffer where the final string will be stored
//              included the NULL terminator. Newly created
//              buffer must be all initialized to 0.
//    size    - size of buffer `buf`
//    src     - string to join
//    delims  - string to add between joined strings
//
// Example of using `gear_strjoin` function:
//
// char buf[1024] = {0};
// gear_strjoin(buf, sizeof(buf), "/usr", "/");
// gear_strjoin(buf, sizeof(buf), "bin", "/");
// printf("%s\n", buf);
//
// result: /usr/bin
void gear_strjoin(char *buf, size_t size, const char *src, const char *delims);
bool gear_strstartswith(const char *str, const char *prefix);
bool gear_strendswith(const char *str, const char *suffix);
// void gear_strreplace(char *buf, size_t size, size_t memsize, char *str, const char *replacee, const char *replacement):
//    buf           - buffer where the final string will be stored
//                    included the NULL terminator. Newly created
//                    buffer must be all initialized to 0.
//    size          - size of buffer `buf`.
//    msize         - max size of substring included NULL terminator
//                    (see implementation).
//    str           - string to operate on.
//    replacee      - what to replace.
//    replacement   - replace with what.
//
// Example of using `gear_strreplace` function:
//
// char buf[1024] = {0};
// gear_strreplace(buf, sizeof(buf), 32, "hello, hi, ciao, hola", ", ", "|");
// printf("%s\n", buf);
//
// result: hello|hi|ciao|hola
void gear_strreplace(char *buf, size_t size, size_t memsize, char *str, const char *replacee, const char *replacement);

#define GEAR_STRCAPITALIZE(str) (str)[0] = toupper((str)[0])
#define GEAR_STRLOWER(str) for (size_t i = 0; i < strlen(str); i++) (str)[i] = tolower((str)[i])
#define GEAR_STRUPPER(str) for (size_t i = 0; i < strlen(str); i++) (str)[i] = toupper((str)[i])

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

GearMap *gear_map_init(const GearMapType key_type, const size_t capacity);
void gear_map_insert(GearMap *map, void *key, void *value);
int gear_map_remove(const GearMap *map, void *key);
void *gear_map_find(const GearMap *map, void *key);
// This set of functions operates on typedef struct GearMapSlot and
// behaves differently with each function, so do read the comments
// under each of them.
// TODO: implement them as macros?
//
// Frees slot->key slot->value and slot itself.
// May be useful for hashmaps that have both key and value as a
// dynamically allocated memory.
void gear_map_delete_kvpairs(GearMap *map);
// Frees slot->key and slot itself.
// May be useful for hashmaps that have constant string literals
// as values and dynamically allocated memory as key.
void gear_map_delete_keys(GearMap *map);
// Frees slot->value and slot itself.
// May be useful for hashmaps that have constant string literals
// as keys and dynamically allocated memory as value.
void gear_map_delete_values(GearMap *map);
// Frees slot itself.
// May be useful for hashmaps that contain only constant string
// literals.
void gear_map_delete_slots(GearMap *map);

#define GEAR_MAP_GET(map, _type, key) *(_type *)gear_map_find(map, key)
#define GEAR_MAP_GET_STRING(map, key) (char *)gear_map_find(map, key)
#define GEAR_MAP_EXISTS(map, key) gear_map_find(map, key) != NULL
#define GEAR_MAP_DELETE(map) \
  do {                  \
    free(map->slots); \
    free(map);          \
  } while (0)

#endif
