#include <assert.h>
#include "../mygear.h"

int main(void)
{
  size_t size;
  char *content = read_entire_file("test/io.c", &size);
  assert(size == 186); // length of this file.
  free(content);
  return 0;
}
