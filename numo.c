#include<termios.h>
#include<unistd.h>
#include<stdlib.h>
#include<ctype.h>
#include<stdio.h>

struct termios orig_termios;//global struct

void disableRawMode()
{
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

/*
By default your terminal starts in canonical mode, also called cooked mode. In this mode, keyboard input is only sent to your program when
the user presses Enter. This is useful for many programs: it lets the user type in a line of text, use Backspace to fix errors until they
get their input exactly the way they want it, and finally press Enter to send it to the program. But it does not work well for programs with
more complex user interfaces, like text editors. We want to process each keypress as it comes in, so we can respond to it immediately.
What we want is raw mode. Unfortunately, there is no simple switch you can flip to set the terminal to raw mode. Raw mode is achieved by
turning off a great many flags in the terminal.Major thing is we want to process the keyboard input as we type, which is possible only in raw
mode and not in canonical mode.

here we are setting the terminal's attributes by turning off ECHO feature(means it nomore prints in terminal -> only when program is running,
not for other commands)
*/
void enableRawMode()
{
  tcgetattr(STDIN_FILENO, &orig_termios);
  atexit(disableRawMode);

  struct termios raw = orig_termios;//copying orig_termios to raw.
  raw.c_lflag &= ~(ECHO | ICANON); //Because of ICANON program will quit as soon as you press 'q'. Now the program doesn't wait for you to press 'q' and
                                   //then press 'ENTER' to quit. This means we will finally be reading input byte-by-byte, instead of line-by-line.
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw); //TCSAFLUSH -> doesn't take any input after we press 'q', it just ignores it.
}



int main()
{
  enableRawMode();

  char c;
  while(read(STDIN_FILENO, &c, 1) == 1 && c!='q') //1 here is one byte..we are telling it to read 1 byte in loop until 'q' is typed
  {
    if(iscntrl(c))
    {
      printf("%d\n", c);
    }
    else
    {
      printf("%d ('%c')\n", c, c);
    }
  }

  return 0;
}
