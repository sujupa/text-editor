#include<termios.h>
#include<unistd.h>
#include<stdlib.h>
#include<ctype.h>
#include<stdio.h>
#include<errno.h>

struct termios orig_termios;//global structure

void die(const char *s)
{
  perror(s);
  exit(1);
}

void disableRawMode()
{
  if(tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
  die("tcsetattr");
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
  if(tcgetattr(STDIN_FILENO, &orig_termios) == -1) die("tcgetattr");
  atexit(disableRawMode);

  struct termios raw = orig_termios;//copying orig_termios to raw.
  raw.c_lflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);//IXON turns off 'Ctrl-S' and 'Ctrl-Q' signals, where 'Ctrl-S' stops data from being transmitted to the terminal until you press 'Ctrl-Q'.
  raw.c_lflag &= ~(OPOST); //OPOST turns off all output processing features by turning off the OPOST flag. In practice, the "\n" to "\r\n" translation is likely the only
                           //output processing feature turned on by default.
  raw.c_lflag &= ~(ECHO | ICANON | IEXTEN |ISIG); //Because of ICANON program will quit as soon as you press 'q'. Now the program doesn't wait for you to press 'q' and
                                   //then press 'ENTER' to quit. This means we will finally be reading input byte-by-byte, instead of line-by-line.
                                   //ISIG turns off 'Ctrl-C' and 'Ctrl-Z' signals which used to terminate the programs
                                   //IEXTEN turns off the terminal which waits for you to type another character and then sends that character literally.
                                   //ICRNL turns off the terminal which is helpfully translating any carriage returns (13, '\r') inputted by the user into newlines (10, '\n') and
                                   //now Ctrl-M is read as a 13 (carriage return), and the Enter key is also read as a 13.
  raw.c_cc[VMIN] = 0;
  raw.c_cc[VTIME] = 1;
  if(tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) die("tcsetattr"); //TCSAFLUSH -> doesn't take any input after we press 'q', it just ignores it.
}



int main()
{
  enableRawMode();

  while(1) //1 here is one byte..we are telling it to read 1 byte in loop until 'q' is typed
  {
    char c = '\0';
    if(read(STDIN_FILENO, &c, 1) == -1 && errno != EAGAIN) die("read");
    /* iscntrl() tests whether a character is a control character. Control characters are nonprintable characters that we don’t want to
    print to the screen. ASCII codes 0–31 are all control characters, and 127 is also a control character. ASCII codes 32–126 are all
    printable.*/
    if(iscntrl(c))
    {
      printf("%d\n", c);
    }
    else
    {
      printf("%d ('%c')\r\n", c, c);
    }
    if(c == 'q')//'q' is to quit
    {
      break;
    }
  }

  return 0;
}
