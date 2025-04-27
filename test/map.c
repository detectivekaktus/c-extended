#define _XOPEN_SOURCE 500
#include "../gear.h"

void strstrmap(void)
{
  GearMap *map = gear_map_init(MAP_TYPE_STRING, 64);
  gear_map_insert(map, "hello", "world");
  gear_map_insert(map, "ciao", "mondo");
  gear_map_insert(map, "hola", "mundo");

  assert(GEAR_MAP_EXISTS(map, "hello"));
  printf("%s %s\n", "hello", GEAR_MAP_GET_STRING(map, "hello"));
  assert(GEAR_MAP_EXISTS(map, "ciao"));
  printf("%s %s\n", "ciao", GEAR_MAP_GET_STRING(map, "ciao"));
  assert(GEAR_MAP_EXISTS(map, "hola"));
  printf("%s %s\n", "hola", GEAR_MAP_GET_STRING(map, "hola"));

  gear_map_delete_slots(map);
  GEAR_MAP_DELETE(map);
}

void intstrmap(void)
{
  GearMap *map = gear_map_init(MAP_TYPE_INTEGER, 64);
  int *zero = GEAR_NEW(int, 1);
  *zero = 0;
  gear_map_insert(map, zero, "zero");
  int *one = GEAR_NEW(int, 1);
  *one = 1;
  gear_map_insert(map, one, "one");
  int *two = GEAR_NEW(int, 1);
  *two = 2;
  gear_map_insert(map, two, "two");

  assert(GEAR_MAP_EXISTS(map, zero));
  printf("%d %s\n", 0, GEAR_MAP_GET_STRING(map, zero));
  assert(GEAR_MAP_EXISTS(map, one));
  printf("%d %s\n", 1, GEAR_MAP_GET_STRING(map, one));
  assert(GEAR_MAP_EXISTS(map, two));
  printf("%d %s\n", 2, GEAR_MAP_GET_STRING(map, two));

  gear_map_delete_keys(map);
  GEAR_MAP_DELETE(map);
}

void strintmap(void)
{
  GearMap *map = gear_map_init(MAP_TYPE_STRING, 2);
  char strs[][16] = {
    "zero",
    "one",
    "two",
    "three",
    "four"
  };
  for (int i = 0; i < 5; i++) {
    int *n = GEAR_NEW(int, 1);
    *n = i;
    gear_map_insert(map, strdup(strs[i]), n);
  }

  assert(map->capacity == 8);

  for (int i = 0; i < 5; i++) {
    assert(GEAR_MAP_EXISTS(map, strs[i]));
    printf("%s: %d\n", strs[i], GEAR_MAP_GET(map, int, strs[i]));
  }

  gear_map_delete_kvpairs(map);
  GEAR_MAP_DELETE(map);
}


int main(void)
{
  strstrmap();
  intstrmap();
  strintmap();
  return 0;
}
