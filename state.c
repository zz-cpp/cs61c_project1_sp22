#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "snake_utils.h"
#include "state.h"

/* Helper function definitions */
static char get_board_at(game_state_t *state, int x, int y);
static void set_board_at(game_state_t *state, int x, int y, char ch);
static bool is_tail(char c);
static bool is_snake(char c);
static char body_to_tail(char c);
static int incr_x(char c);
static int incr_y(char c);
static void find_head(game_state_t *state, int snum);
static char next_square(game_state_t *state, int snum);
static void update_tail(game_state_t *state, int snum);
static void update_head(game_state_t *state, int snum);

/* Helper function to get a character from the board (already implemented for you). */
static char get_board_at(game_state_t *state, int x, int y)
{
  return state->board[y][x];
}

/* Helper function to set a character on the board (already implemented for you). */
static void set_board_at(game_state_t *state, int x, int y, char ch)
{
  state->board[y][x] = ch;
}

/* Task 1 */
game_state_t *create_default_state()
{
  // TODO: Implement this function.

  game_state_t *default_game_state = (game_state_t *)malloc(sizeof(game_state_t));
  snake_t *one = (snake_t *)malloc(sizeof(snake_t));
  int num_row = 10;
  default_game_state->board = malloc(sizeof(char *) * 10);
  for (int i = 0; i < 9; i++)
  {
    default_game_state->board[i] = malloc(sizeof(char) * 15);
  }

  default_game_state->board[9] = malloc(sizeof(char) * 15 + 1);

  strcpy(default_game_state->board[0], "##############");

  for (int i = 1; i < num_row - 1; i++)
  {
    strcpy(default_game_state->board[i], "#            #");
  }

  strcpy(default_game_state->board[9], "##############\n");

  default_game_state->x_size = 14;
  default_game_state->y_size = 10;
  default_game_state->num_snakes = 1;

  one->head_x = 5;
  one->head_y = 4;
  one->tail_x = 4;
  one->tail_y = 4;
  one->live = true;
  default_game_state->snakes = one;

  set_board_at(default_game_state, 4, 4, 'd');
  set_board_at(default_game_state, 5, 4, '>');
  set_board_at(default_game_state, 9, 2, '*');

  return default_game_state;
}

/* Task 2 */
void free_state(game_state_t *state)
{
  // TODO: Implement this function.

  int y_board = state->y_size;

  for (int i = 0; i < y_board; i++)
  {
    free(state->board[i]);
  }




  return;
}

/* Task 3 */
void print_board(game_state_t *state, FILE *fp)
{
  // TODO: Implement this function.
  int board_row = state->y_size;
  for (int i = 0; i < board_row; i++)
  {
    fprintf(fp, "%s\n", *(state->board + i));
  }

  return;
}

/* Saves the current state into filename (already implemented for you). */
void save_board(game_state_t *state, char *filename)
{
  FILE *f = fopen(filename, "w");
  print_board(state, f);
  fclose(f);
}

/* Task 4.1 */
static bool is_tail(char c)
{
  // TODO: Implement this function.
  switch (c)
  {
  case 'w':
    return true;
    break;
  case 'a':
    return true;
    break;
  case 's':
    return true;
  case 'd':
    return true;
  default:
    return false;
  }
}

static const char *snake_chars = "wasd<>v^x";
static bool is_snake(char c)
{
  // TODO: Implement this function.

  int index_num = 8;

  for (int i = 0; i < index_num; i++)
  {
    if (snake_chars[i] == c)
    {
      return true;
    }
  }

  return false;
}

static char body_to_tail(char c)
{
  // TODO: Implement this function.
  switch (c)
  {
  case '^':
    return 'w';
    break;
  case '<':
    return 'a';
    break;
  case '>':
    return 'd';
    break;
  case 'v':
    return 's';
    break;
  default:
    return '?';
    break;
  }
}

static int incr_x(char c)
{
  // TODO: Implement this function.
  if (c == '>' || c == 'd')
  {
    return 1;
  }

  if (c == '<' || c == 'a')
  {
    return -1;
  }

  return 0;
}

static int incr_y(char c)
{
  // TODO: Implement this function.

  if (c == 'v' || c == 's')
  {
    return 1;
  }

  if (c == '^' || c == 'w')
  {
    return -1;
  }

  return 0;
}

/* Task 4.2 */
static char next_square(game_state_t *state, int snum)
{

  // choose a sname according to snum
  snake_t *choosing = state->snakes + snum;

  int col = choosing->head_x;
  int row = choosing->head_y;

  char head = get_board_at(state, col, row);
  int incrx = incr_x(head);
  int incry = incr_y(head);

  char moving_into = get_board_at(state, col + incrx, row + incry);

  return moving_into;
}

/* Task 4.3 */
static void update_head(game_state_t *state, int snum)
{
  // TODO: Implement this function.

  snake_t *choosing = state->snakes + snum;

  int col = choosing->head_x;

  int row = choosing->head_y;

  char head = get_board_at(state, col, row);

  int incrx = incr_x(head);

  int incry = incr_y(head);

  col = col + incrx;

  row = row + incry;

  choosing->head_x = col;

  choosing->head_y = row;

  set_board_at(state, col, row, head);

  return;
}

/* Task 4.4 */
static void update_tail(game_state_t *state, int snum)
{
  // TODO: Implement this function.

  snake_t *choosing = state->snakes + snum;

  int col = choosing->tail_x;

  int row = choosing->tail_y;

  char tail = get_board_at(state, col, row);

  int incrx = incr_x(tail);

  int incry = incr_y(tail);

  // clear orign
  set_board_at(state, col, row, ' ');

  // update col and row
  col = col + incrx;

  row = row + incry;

  char new_tail = body_to_tail(get_board_at(state, col, row));

  choosing->tail_x = col;

  choosing->tail_y = row;

  // the tail will change by the head changing

  set_board_at(state, col, row, new_tail);

  return;
}

/* Task 4.5 */
void update_state(game_state_t *state, int (*add_food)(game_state_t *state))
{
  // TODO: Implement this function.

  unsigned int count = state->num_snakes;

  // loop update every snakes
  for (int i = 0; i < count; i++)
  {
    char next_char = next_square(state, i);

    // next char is fruit
    if (next_char == '*')
    {

      update_head(state, i);

      add_food(state);

      continue;
    }

    // next the snake will die
    if (is_snake(next_char) || next_char == '#')
    {

      (state->snakes + i)->live = false;

      set_board_at(state, (state->snakes + i)->head_x, (state->snakes +i)->head_y, 'x');

      continue;
    }

    update_head(state, i);

    update_tail(state, i);
  }

  return;
}

/* Task 5 */
game_state_t *load_board(char *filename)
{
  // TODO: Implement this function.

  FILE *game_file = fopen(filename, "r");

  game_state_t *game_state = malloc(sizeof(game_state));

  unsigned int rows = 0;

  unsigned int cols = 0;

  char buffer[1000] = {'\0'};

  // count number of file row and col;

  int count = 0;
  while (fgets(buffer, sizeof(buffer), game_file) != NULL)
  {

    if (count == 0)
    {
      cols = strlen(buffer);
      count++;
    }
    rows++;
  }

  game_state->board = malloc(sizeof(char *) * rows);

  // gurante it has a '/n' and '/0' so cols + 2
  for (int i = 0; i < rows; i++)
  {
    *(game_state->board + i) = malloc(sizeof(char) * cols + 1 + 1);
  }

  rewind(game_file);

  for (int i = 0; i < rows; i++)
  {
    // every line have to read two more than '/n','/0';

    fgets(*(game_state->board + i), cols + 2, game_file);
    // remove '/n'
    game_state->board[i][strcspn(*(game_state->board + i), "\n")] = 0;
  }

  game_state->x_size = cols - 1;
  game_state->y_size = rows;

  fclose(game_file);

  return game_state;
}

/* Task 6.1 */
static void find_head(game_state_t *state, int snum)
{
  // TODO: Implement this function.

  // movw from snake tail to head

  int incrx = 0;

  int incry = 0;

  snake_t *snake = state->snakes + snum;

  int s_x = snake->tail_x;

  int s_y = snake->tail_y;

  char now_char = get_board_at(state, s_x, s_y);

  while (is_snake(now_char))
  {
    incrx = incr_x(now_char);

    incry = incr_y(now_char);

    s_x += incrx;

    s_y += incry;

    now_char = get_board_at(state, s_x, s_y);
  }

  s_x -= incrx;

  s_y -= incry;

  snake->head_x = s_x;

  snake->head_y = s_y;

  return;
}

/* Task 6.2 */

/*
 *  problem : why can't use 
 * */
game_state_t *initialize_snakes(game_state_t *state)
{
  // TODO: Implement this function.

  int cols = state->x_size;

  int rows = state->y_size;

  char** board = state->board;

  int snake_num = 0;

  int xarray[10] = {0};

  int yarray[10] = {0};

// scanf all board to create snake through finding tail

  for (int i = 0; i < rows; i++)
  {
    for (int j = 0; j < cols; j++)
    {
       if(is_tail(board[i][j])){

           snake_num++;

           yarray[snake_num] = i;

           xarray[snake_num] = j;


       }
    }
  }

    state->snakes = malloc(sizeof(snake_t) * snake_num);

    for (int i = 0; i < snake_num; i++ ) {

        state->snakes[i].tail_x = xarray[i + 1];

        state->snakes[i].tail_y = yarray[i + 1];

        state->snakes[i].live = true;

        find_head(state,i);
    }

    state->num_snakes = snake_num;

  return state;
}
