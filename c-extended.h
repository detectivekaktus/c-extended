////////////////////////////////////////////////////////////////////
//                                                                //
// C-extended is a header only library that aims to increase the  //
// productivity of C developers and provide some boilerplate code //
// code that you would've written anyway in your project.         //
//                                                                //
// Copyright 2024 (C) Artiom Astashonak                           //
// https://github.com/detectivekaktus/c-extended                  //
//                                                                //
////////////////////////////////////////////////////////////////////
#ifdef EXTEND_C

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>
#include <string.h>

typedef struct {
  int argc;
  char **argv;
  size_t index;
} Args;

#define init_args(argc, argv) {.argc=argc, .argv=argv, .index=1}
#define shift_args(args) (args)->argv[(args)->index++]

// This function requires you to free manually the string you get
// back after calling the function.
char *read_entire_file(const char *filename);

#define CRASH(msg)        \
  do {                    \
    fprintf(stderr, msg); \
    exit(1);              \
  } while (0)

typedef enum {
  C_DEBUG,
  C_INFO,
  C_WARNING,
  C_ERROR
} Log_Level;

void c_log(Log_Level level, const char *msg, ...);

// Dynamic arrays must have the following definition format:
// struct {
//   void *items;     --> pointer to a precise type to the items.
//   size_t size;     --> size_t size of the dynamic array.
//   size_t capacity; --> size_t capacity of the dynamic array.
// } Array;
//
// All the dynamic array macros require you to pass the dynamic
// array as a pointer.
#define DA_INIT_CAPACITY 64

typedef struct {
  char *items;
  size_t size;
  size_t capacity;
} CString_Builder;

#define da_heap_alloc(type) (type*) calloc(1, sizeof(type))

#define da_append(da, item)                                                         \
  do {                                                                              \
    if ((da)->size >= (da)->capacity) {                                             \
      (da)->capacity = (da)->capacity == 0 ? DA_INIT_CAPACITY : (da)->capacity * 2; \
      (da)->items = realloc((da)->items, (da)->capacity * sizeof(*(da)->items));    \
      if ((da)->items == NULL) CRASH("buy more ram, lol\n");                        \
    }                                                                               \
    (da)->items[(da)->size++] = (item);                                             \
  } while (0)

#define da_free(da) free((da)->items);

#define da_heap_free(da)  \
  do {                    \
    free((da)->items);    \
    free(da);             \
  } while (0)

#define sb_append_str(sb, str)                                                        \
  do {                                                                                \
    if ((sb)->size + strlen(str) + 1 >= (sb)->capacity) {                             \
      while ((sb)->size + strlen(str) + 1>= (sb)->capacity)                           \
        (sb)->capacity = (sb)->capacity == 0 ? DA_INIT_CAPACITY : (sb)->capacity * 2; \
      (sb)->items = realloc((sb)->items, (sb)->capacity);                             \
      if ((sb)->items == NULL) CRASH("buy more ram, lol\n");                          \
    }                                                                                 \
    (sb)->size -= (sb)->size == 0 ? 0 : 1;                                            \
    for (int i = 0; i < strlen(str) + 1; i++)                                         \
      (sb)->items[(sb)->size++] = str[i];                                             \
  } while (0)


char *read_entire_file(const char *filename)
{
  FILE *f = fopen(filename, "r");
  if (f == NULL) return NULL;
  fseek(f, 0, SEEK_END);
  int len = ftell(f);
  fseek(f, 0, SEEK_SET);

  char *str = malloc(sizeof(char) * (len + 1));
  if (str == NULL) CRASH("buy more ram, lol\n");
  char c;
  int i = 0;
  while ((c = fgetc(f)) != EOF) {
    str[i++] = c;
  }
  str[i] = '\0';
  fclose(f);
  return str;
}

void c_log(Log_Level level, const char *msg, ...)
{
  FILE *stream = stdout;
  switch (level) {
    case C_DEBUG: {
      fprintf(stream, "[DEBUG] ");
    } break;
    case C_INFO: {
      fprintf(stream, "\033[32m[INFO]\033[0m ");
    } break;
    case C_WARNING: {
      stream = stderr;
      fprintf(stream, "\033[33m[WARNING]\033[0m ");
    } break;
    case C_ERROR: {
      stream = stderr;
      fprintf(stream, "\033[31m[ERROR]\033[0m ");
    } break;
  }
  va_list args;
  va_start(args, msg);
  vfprintf(stream, msg, args);
  va_end(args);
}

#endif
