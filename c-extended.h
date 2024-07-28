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

// This function requires you to free manually the string you get
// back after calling the function.
char *read_entire_file(const char *filename);

#define CRASH(msg, ...)                   \
  do {                                    \
    fprintf(stderr, msg, ##__VA_ARGS__);  \
    exit(1);                              \
  } while (0)

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

#define da_heap_alloc(type) (type*) calloc(1, sizeof(type));

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


char *read_entire_file(const char *filename)
{
  FILE *f = fopen(filename, "r");
  if (f == NULL) return NULL;
  fseek(f, 0, SEEK_END);
  int len = ftell(f);
  fseek(f, 0, SEEK_SET);

  char *str = malloc(sizeof(char) * (len + 1));
  char c;
  int i = 0;
  while ((c = fgetc(f)) != EOF) {
    str[i++] = c;
  }
  str[i] = '\0';
  fclose(f);
  return str;
}

#endif
