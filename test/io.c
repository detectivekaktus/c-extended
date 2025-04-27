#include <assert.h>
#include "../gear.h"

int main(void)
{
  assert(gear_is_file("test/io.c"));
  assert(gear_is_dir("test"));
  assert(!gear_is_dir_empty("test"));
  return 0;
}
