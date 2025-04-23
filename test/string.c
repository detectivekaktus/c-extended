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
    printf("`%s`\n", trim_leading(leadings[i]));
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
    trim_trailing(str);
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
    char *str = trim_leading(combined[i]);
    trim_trailing(str);
    printf("`%s`\n", str);
  }
}

void splitting(void)
{
  SplitStrings first = {0};
  strsplit(&first, 32, "1,2,3,4,5,6,7", ",");
  for (size_t i = 0; i < first.size; i++)
    printf("`%s`\n", first.items[i]);
  ARRAY_DELETE_ALL(&first);

  SplitStrings second = {0};
  strsplit(&second, 32, "hi, hello, ciao, hola", ", ");
  for (size_t i = 0; i < second.size; i++)
    printf("`%s`\n", second.items[i]);
  ARRAY_DELETE_ALL(&second);
}

void appending(void)
{
  char buf[1024] = {0};
  strappend(buf, sizeof(buf), "hello ");
  strappend(buf, sizeof(buf), "world!");
  printf("%s\n", buf);
}

void joining(void)
{
  char buf[1024] = {0};
  strjoin(buf, sizeof(buf), "/usr", "/");
  strjoin(buf, sizeof(buf), "bin", "/");
  printf("%s\n", buf);
}

void startend(void)
{
  assert(strstartswith("/usr/bin", "/usr"));
  assert(!strstartswith("/usr/bin", "/bin"));

  assert(strendswith("string", "ing"));
  assert(!strendswith("string", "cat"));

  assert(strstartswith("/usr/bin", "/usr") && strendswith("/usr/bin", "/bin"));
}

void replacing(void)
{
  char buf1[1024] = {0};
  strreplace(buf1, sizeof(buf1), 32, "1,2,3,4,5", ",", "|");
  printf("%s\n", buf1);

  char buf2[1024] = {0};
  strreplace(buf2, sizeof(buf2), 32, "1,2,3,4,5,", ",", "|");
  printf("%s\n", buf2);

  char buf3[1024] = {0};
  strreplace(buf3, sizeof(buf3), 32, "hello, hi, ciao, hola", ", ", ",");
  printf("%s\n", buf3);

  char buf4[1024] = {0};
  strreplace(buf4, sizeof(buf4), 32, "hello,hi,ciao,hola", ",", ", ");
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
