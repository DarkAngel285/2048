#include "termios_init.h"
#include "term_tweaks.h"
#include <stdio.h>
#include <unistd.h>

void init(struct termios *oldt, struct termios *newt) {
  curhide(1);
  clear();
  fflush(stdout);
  tcgetattr(STDIN_FILENO, oldt);
  *newt = *oldt;
  newt->c_lflag &= ~(ICANON | ECHO);
  newt->c_cc[VMIN] = 0;
  newt->c_cc[VTIME] = 0;
  tcsetattr(STDIN_FILENO, TCSANOW, newt);
}

void deinit(struct termios *oldt) {
  clear();
  fflush(stdout);
  curhide(0);
  tcsetattr(STDIN_FILENO, TCSANOW, oldt);
}
