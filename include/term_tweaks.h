#ifndef __TERM_TWEAKS_H__
#define __TERM_TWEAKS_H__

#define clear() printf("\x1b[H\x1b[J")
#define gotoxy(x, y) printf("\x1b[%d;%dH", y, x)
#define curhide(i) printf(i ? "\e[?25l" : "\e[?25h")

#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define YELLOW "\x1b[33m"
#define STD "\x1b[0m"

#endif
