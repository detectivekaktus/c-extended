#define _XOPEN_SOURCE 500
#include <assert.h>
#include <ctype.h>
#include "mygear.h"

char *read_entire_file(const char *filename, size_t *size)
{
  char *buffer = NULL;
  FILE *f = fopen(filename, "rb");
  if (f == NULL) {
    fprintf(stderr, "Could not open the file %s: %s\n",
            filename, strerror(errno));
    goto error;
  }

  if (fseek(f, 0, SEEK_END) != 0)
    goto error;
  int n = ftell(f);
  if (fseek(f, 0, SEEK_SET) != 0)
    goto error;

  buffer = malloc(sizeof(char) * n);
  size_t m = fread(buffer, sizeof(char), n, f);
  assert(m == (size_t) n);
  *size = m;
  if (ferror(f) != 0)
    goto error;

  fclose(f);
  return buffer;

error:
  if (f)
    fclose(f);
  if (buffer)
    free(buffer);
  return NULL;
}

char *trim_leading(char *str)
{
  while (isspace(*str)) str++;
  return str;
}

void trim_trailing(char *str)
{
  size_t n = strlen(str) - 1;
  if (n <= 0) return;
  while (isspace(str[n])) n--;
  str[n + 1] = '\0';
}

void strsplit(SplitStrings *arr, size_t size, char *str, const char *delims)
{
  assert(delims != NULL);
  size_t delims_len = strlen(delims);
  char *prev = str;
  char *substr = strstr(str, delims);
  while (substr != NULL) {
    char buf[size];
    size_t n = substr - prev;
    assert(n < size);
    memcpy(buf, prev, substr - prev);
    buf[n] = '\0';
    ARRAY_APPEND(arr, strdup(buf));
    prev = substr + delims_len;
    substr = strstr(prev, delims);
  }
  char buf[size];
  size_t n = strlen(prev);
  assert(n < size);
  memcpy(buf, prev, n);
  buf[n] = '\0';
  ARRAY_APPEND(arr, strdup(buf));
}
