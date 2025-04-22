#include "../mygear.h"

int main(void)
{
  size_t size;
  char *content = read_entire_file("test/io.c", &size);
  printf("%s\n", content);
  free(content);
  return 0;
}
