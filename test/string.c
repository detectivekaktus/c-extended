#include <assert.h>
#include "../gear.h"

void leading(void)
{
  char leadings[][32] = {
    "    leading",
    "  leading",
    "leading",
    "leading   ",
    ""
  };
  for (int i = 0; i < 5; i++)
    printf("`%s`\n", gear_trim_leading(leadings[i]));
}

void trailing(void)
{
  char trailings[][32] = {
    "trailing    ",
    "trailing   ",
    "trailing",
    "    trailing",
    ""
  };
  for (int i = 0; i < 5; i++) {
    char *str = trailings[i];
    gear_trim_trailing(str);
    printf("`%s`\n", str);
  }
}

void both(void)
{
  char combined[][32] = {
    "  combo   ",
    "combo  ",
    "   combo",
    "\tcombo",
    "combo",
    "combo\t",
    ""
  };
  for (int i = 0; i < 7; i++) {
    char *str = gear_trim_leading(combined[i]);
    gear_trim_trailing(str);
    printf("`%s`\n", str);
  }
}

void splitting(void)
{
  GearSplitStrings first = {0};
  gear_strsplit(&first, 32, "1,2,3,4,5,6,7", ",");
  for (size_t i = 0; i < first.size; i++)
    printf("`%s`\n", first.items[i]);
  GEAR_ARRAY_DELETE_ALL(&first);

  GearSplitStrings second = {0};
  gear_strsplit(&second, 32, "hi, hello, ciao, hola", ", ");
  for (size_t i = 0; i < second.size; i++)
    printf("`%s`\n", second.items[i]);
  GEAR_ARRAY_DELETE_ALL(&second);
}

void appending(void)
{
  char buf[1024] = {0};
  gear_strappend(buf, sizeof(buf), "hello ");
  gear_strappend(buf, sizeof(buf), "world!");
  printf("%s\n", buf);
}

void joining(void)
{
  char buf[1024] = {0};
  gear_strjoin(buf, sizeof(buf), "/usr", "/");
  gear_strjoin(buf, sizeof(buf), "bin", "/");
  printf("%s\n", buf);
}

void startend(void)
{
  assert(gear_strstartswith("/usr/bin", "/usr"));
  assert(!gear_strstartswith("/usr/bin", "/bin"));

  assert(gear_strendswith("string", "ing"));
  assert(!gear_strendswith("string", "cat"));

  assert(gear_strstartswith("/usr/bin", "/usr") && gear_strendswith("/usr/bin", "/bin"));
}

void replacing(void)
{
  char buf1[1024] = {0};
  gear_strreplace(buf1, sizeof(buf1), 32, "1,2,3,4,5", ",", "|");
  printf("%s\n", buf1);

  char buf2[1024] = {0};
  gear_strreplace(buf2, sizeof(buf2), 32, "1,2,3,4,5,", ",", "|");
  printf("%s\n", buf2);

  char buf3[1024] = {0};
  gear_strreplace(buf3, sizeof(buf3), 32, "hello, hi, ciao, hola", ", ", ",");
  printf("%s\n", buf3);

  char buf4[1024] = {0};
  gear_strreplace(buf4, sizeof(buf4), 32, "hello,hi,ciao,hola", ",", ", ");
  printf("%s\n", buf4);
}

int main(void)
{
  leading();
  trailing();
  both();
  splitting();

  appending();
  joining();
  replacing();
  return 0;
}
