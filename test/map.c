#define _XOPEN_SOURCE 500
#include "../gear.h"

void strstrmap(void)
{
  GearMap *map = map_init(MAP_TYPE_STRING, 64);
  map_insert(map, "hello", "world");
  map_insert(map, "ciao", "mondo");
  map_insert(map, "hola", "mundo");

  assert(MAP_EXISTS(map, "hello"));
  printf("%s %s\n", "hello", MAP_GET_STRING(map, "hello"));
  assert(MAP_EXISTS(map, "ciao"));
  printf("%s %s\n", "ciao", MAP_GET_STRING(map, "ciao"));
  assert(MAP_EXISTS(map, "hola"));
  printf("%s %s\n", "hola", MAP_GET_STRING(map, "hola"));

  map_delete_slots(map);
  MAP_DELETE(map);
}

void intstrmap(void)
{
  GearMap *map = map_init(MAP_TYPE_INTEGER, 64);
  int *zero = GEAR_NEW(int, 1);
  *zero = 0;
  map_insert(map, zero, "zero");
  int *one = GEAR_NEW(int, 1);
  *one = 1;
  map_insert(map, one, "one");
  int *two = GEAR_NEW(int, 1);
  *two = 2;
  map_insert(map, two, "two");

  assert(MAP_EXISTS(map, zero));
  printf("%d %s\n", 0, MAP_GET_STRING(map, zero));
  assert(MAP_EXISTS(map, one));
  printf("%d %s\n", 1, MAP_GET_STRING(map, one));
  assert(MAP_EXISTS(map, two));
  printf("%d %s\n", 2, MAP_GET_STRING(map, two));

  map_delete_keys(map);
  MAP_DELETE(map);
}

void strintmap(void)
{
  GearMap *map = map_init(MAP_TYPE_STRING, 2);
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
    map_insert(map, strdup(strs[i]), n);
  }

  assert(map->capacity == 8);

  for (int i = 0; i < 5; i++) {
    assert(MAP_EXISTS(map, strs[i]));
    printf("%s: %d\n", strs[i], MAP_GET(map, int, strs[i]));
  }

  map_delete_kvpairs(map);
  MAP_DELETE(map);
}


int main(void)
{
  strstrmap();
  intstrmap();
  strintmap();
  return 0;
}
