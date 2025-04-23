#include <assert.h>
#include "../gear.h"

int main(void)
{
  assert(is_file("test/io.c"));
  assert(is_dir("test"));
  assert(!is_dir_empty("test"));
  return 0;
}
