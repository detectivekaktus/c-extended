#include <assert.h>
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
