#include "../mygear.h"

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
  strsplit(&first, "1,2,3,4,5,6,7", ",");
  for (size_t i = 0; i < first.size; i++)
    printf("`%s`\n", first.items[i]);
  ARRAY_DELETE_ALL(&first);

  SplitStrings second = {0};
  strsplit(&second, "hi, hello, ciao, hola", ", ");
  for (size_t i = 0; i < second.size; i++)
    printf("`%s`\n", second.items[i]);
  ARRAY_DELETE_ALL(&second);
}

int main(void)
{
  leading();
  trailing();
  both();
  splitting();
  return 0;
}
