#include<unistd.h>

/*
By default your terminal starts in canonical mode, also called cooked mode. In this mode, keyboard input is only sent to your program when
the user presses Enter. This is useful for many programs: it lets the user type in a line of text, use Backspace to fix errors until they
get their input exactly the way they want it, and finally press Enter to send it to the program. But it does not work well for programs with
more complex user interfaces, like text editors. We want to process each keypress as it comes in, so we can respond to it immediately.
What we want is raw mode. Unfortunately, there is no simple switch you can flip to set the terminal to raw mode. Raw mode is achieved by
turning off a great many flags in the terminal.Major thing is we want to process the keyboard input as we type, which is possible only in raw
mode and not in canonical mode.
*/
//here we are setting the terminal's attributes
void enableRawMode()
{
  struct termios raw;
  tcgetattr(STDIN_FILENO, &raw);
  raw.c_flag &= ~(ECHO);
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

int main()
{
  enableRawMode();

  char c;
  while(read(STDIN_FILENO, &c, 1) == 1 && c!='q'); //1 here is one byte..we are telling it to read 1 byte in loop until 'q' is typed
  return 0;
}
