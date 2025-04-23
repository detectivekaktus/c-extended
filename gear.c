#define _XOPEN_SOURCE 500
#include <assert.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

#include "gear.h"

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

// TODO: convert this function to a macro?
bool is_file(const char *filepath)
{
  return access(filepath, F_OK) == 0;
}

bool is_dir(const char *dirpath)
{
  DIR *dir = opendir(dirpath);
  if (dir != NULL) {
    closedir(dir);
    return true;
  }
  return false;
}

bool is_dir_empty(const char *dirpath)
{
  DIR *dir = opendir(dirpath);
  if (dir == NULL)
    return false;
  struct dirent *dirent = readdir(dir);
  while (dirent != NULL) {
    if (strcmp(dirent->d_name, ".") == 0 || strcmp(dirent->d_name, "..") == 0)
      dirent = readdir(dir);
    else
      return false;
  }
  return true;
}

int makedir(char *dirpath)
{
  SplitStrings arr = {0};
  strsplit(&arr, GEAR_MAX_DIRNAME_LENGTH, dirpath, "/");
  char buf[GEAR_MAX_DIRNAME_LENGTH * GEAR_MAX_DIR_CHILDREN] = {0};
  for (size_t i = 0; i < arr.size; i++) {
    strjoin(buf, sizeof(buf), arr.items[i], "/");
    if (mkdir(buf, 0777) != 0) {
      ARRAY_DELETE_ALL(&arr);
      return 1;
    }
  }
  ARRAY_DELETE_ALL(&arr);
  return 0;
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

// TODO: return integer instead of asserting the size
void strappend(char *buf, size_t size, const char *src)
{
  assert(size >= (*buf == 0) ? strlen(src) + 1 :
         strlen(buf) + strlen(src) + 1);
  if (*buf == 0)
    memcpy(buf, src, strlen(src) + 1);
  else
    strcat(buf, src);
}

// TODO: return integer instead of asserting the size
void strjoin(char *buf, size_t size, const char *src, const char *delims)
{
  size_t delims_n = strlen(delims);
  assert(size >= (*buf == 0) ? strlen(src) + delims_n + 1 :
         strlen(buf) + strlen(src) + delims_n + 1);
  if (*buf == 0)
    strappend(buf, size, src);
  else {
    strappend(buf, size, delims);
    strappend(buf, size, src);
  }
}

bool strstartswith(const char *str, const char *prefix)
{
  assert(strlen(prefix) <= strlen(str));
  for (size_t i = 0; i < strlen(prefix); i++)
    if (str[i] != prefix[i]) return false;
  return true;
}

bool strendswith(const char *str, const char *suffix)
{
  int n = strlen(str);
  int m = strlen(suffix);
  assert(m <= n);
  for (int i = 0; i > -m; i--)
    if (str[n - i] != suffix[m - i]) return false;
  return true;
}
