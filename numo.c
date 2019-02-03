#include<unistd.h>

int main()
{
  char c;
  while(read(STDIN_FILENO, &c, 1) == 1 && c!='q'); //1 here is one byte..we are telling it to read 1 byte until 'q' is typed
  return 0;
}
