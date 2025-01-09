#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>

#define clear() printf("\x1b[H\x1b[J")
#define gotoxy(x, y) printf("\x1b[%d;%dH", y, x)
#define curhide(i) printf(i ? "\e[?25l" : "\e[?25h")

#define MAXBYTES 3
#define FIELD_SIZE 4
#define UP 0
#define DOWN 1
#define RIGHT 2
#define LEFT 3

#define RED "\x1b[31m"
#define STD "\x1b[0m"

void print_field(int **field) {
  clear();
  for (int i = 0; i < FIELD_SIZE * 4 + 3 * (FIELD_SIZE - 1) + 4; i++)
    printf("%c", '#');
  printf("\n");
  for (int i = 0; i < FIELD_SIZE; i++) {
    printf("#");
    for (int j = 0; j < FIELD_SIZE; j++) {
      if (field[i][j])
        printf(" %s%4d%s #", RED, field[i][j], STD);
      else
        printf(" %4d #", field[i][j]);
    }
    printf("\n");
  }
  for (int i = 0; i < FIELD_SIZE * 4 + 3 * (FIELD_SIZE - 1) + 4; i++)
    printf("%c", '#');
  printf("\n");

  fflush(stdout);
}

int check_win(int **field) {
  for (int i = 0; i < FIELD_SIZE; i++)
    for (int j = 0; j < FIELD_SIZE; j++)
      if (field[i][j] == 2048)
        return 1;
  return 0;
}

void spawn_tile(int **field, unsigned int *seed) {
  int *empty[FIELD_SIZE * FIELD_SIZE], idx = 0;
  for (int y = 0; y < FIELD_SIZE; y++)
    for (int x = 0; x < FIELD_SIZE; x++)
      if (!field[y][x]) {
        empty[idx] = &field[y][x];
        idx++;
      }
  *empty[(unsigned int)(rand_r(seed) % idx)] =
      (unsigned int)(rand_r(seed) % 10 == 0 ? 4 : 2);
}

void move(int **field, unsigned int *seed, int direction) {
  int changed = 0;

  switch (direction) {
  case 0:
    for (int x = 0; x < FIELD_SIZE; x++) {
      int pos = 0;
      for (int y = 0; y < FIELD_SIZE; y++) {
        if (field[y][x] && pos != y) {
          if (field[pos][x]) {
            if (field[pos][x] == field[y][x]) {
              field[pos][x] += field[y][x];
              field[y][x] = 0;
              changed = 1;
            }
            pos++;
            y--;
          } else {
            field[pos][x] = field[y][x];
            field[y][x] = 0;
            changed = 1;
          }
        }
      }
    }
    break;
  case 1:
    for (int x = 0; x < FIELD_SIZE; x++) {
      int pos = FIELD_SIZE - 1;
      for (int y = FIELD_SIZE - 1; y >= 0; y--) {
        if (field[y][x] && pos != y) {
          if (field[pos][x]) {
            if (field[pos][x] == field[y][x]) {
              field[pos][x] += field[y][x];
              field[y][x] = 0;
              changed = 1;
            }
            pos--;
            y++;
          } else {
            field[pos][x] = field[y][x];
            field[y][x] = 0;
            changed = 1;
          }
        }
      }
    }
    break;
  case 2:
    for (int y = 0; y < FIELD_SIZE; y++) {
      int pos = FIELD_SIZE - 1;
      for (int x = FIELD_SIZE - 1; x >= 0; x--) {
        if (field[y][x] && pos != x) {
          if (field[y][pos]) {
            if (field[y][pos] == field[y][x]) {
              field[y][pos] += field[y][x];
              field[y][x] = 0;
              changed = 1;
            }
            pos--;
            x++;
          } else {
            field[y][pos] = field[y][x];
            field[y][x] = 0;
            changed = 1;
          }
        }
      }
    }
    break;
  case 3:
    for (int y = 0; y < FIELD_SIZE; y++) {
      int pos = 0;
      for (int x = 0; x < FIELD_SIZE; x++) {
        if (field[y][x] && pos != x) {
          if (field[y][pos]) {
            if (field[y][pos] == field[y][x]) {
              field[y][pos] += field[y][x];
              field[y][x] = 0;
              changed = 1;
            }
            pos++;
            x--;
          } else {
            field[y][pos] = field[y][x];
            field[y][x] = 0;
            changed = 1;
          }
        }
      }
    }
    break;
  }

  if (changed)
    spawn_tile(field, seed);
}

void free_field(int **field) {
  for (int i = 0; i < FIELD_SIZE; i++)
    free(field[i]);
  free(field);
}

void reset_field(int **field, unsigned int *seed) {
  for (int i = 0; i < FIELD_SIZE; i++)
    for (int j = 0; j < FIELD_SIZE; j++)
      field[i][j] = 0;
  spawn_tile(field, seed);
  spawn_tile(field, seed);
}

int **init_field(unsigned int *seed) {
  int **field = (int **)malloc(FIELD_SIZE * sizeof(int *));
  for (int i = 0; i < FIELD_SIZE; i++) {
    field[i] = (int *)malloc(FIELD_SIZE * sizeof(int));
    for (int j = 0; j < FIELD_SIZE; j++)
      field[i][j] = 0;
  }
  spawn_tile(field, seed);
  spawn_tile(field, seed);
  return field;
}

void game() {
  unsigned int seed = time(0);
  int **field = init_field(&seed);
  print_field(field);
  char c[MAXBYTES];
  while (1) {
    int bytes_read = read(STDIN_FILENO, &c, MAXBYTES);
    if (bytes_read != 0) {
      if ((c[0] == '\x1b' && c[1] == '[' && c[2] == 'A') || c[0] == 'w') {
        move(field, &seed, UP);
      } else if ((c[0] == '\x1b' && c[1] == '[' && c[2] == 'B') ||
                 c[0] == 's') {
        move(field, &seed, DOWN);
      } else if ((c[0] == '\x1b' && c[1] == '[' && c[2] == 'C') ||
                 c[0] == 'd') {
        move(field, &seed, RIGHT);
      } else if ((c[0] == '\x1b' && c[1] == '[' && c[2] == 'D') ||
                 c[0] == 'a') {
        move(field, &seed, LEFT);
      } else if (c[0] == 'r') {
        reset_field(field, &seed);
      } else if (c[0] == 'q') {
        clear();
        break;
      }
      if (check_win(field)) {
        clear();
        printf("You win!\n");
        break;
      }
      print_field(field);
    }
  }
  free_field(field);
}

int main() {
  struct termios oldt, newt;
  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);
  newt.c_cc[VMIN] = 0;
  newt.c_cc[VTIME] = 0;
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  game();
  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  return 0;
}
