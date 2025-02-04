#ifndef __TERMIOS_INIT_H__
#define __TERMIOS_INIT_H__
#include <termios.h>

void init(struct termios *oldt, struct termios *newt);

void deinit(struct termios *oldt);

#endif
