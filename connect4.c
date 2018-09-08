#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define COLS 7
#define ROWS 6


//Gets the uesr's input and returns the collumn they selected
int get_user_input() {
  char *buf = calloc(255, sizeof(char));
  int ret = 0;
  do {
    printf("Which collumn would you like to place your piece in? ");
    fgets(buf, 252, stdin);
    printf("\n");
    ret = atoi(buf);
  } while (ret < 1 || ret > COLS); //catch any invalid inputs
  free(buf);
  return ret;
}

//Properly initializes the board to the correct size
char **init_board() {
  char **board = calloc(ROWS + 1, sizeof(char *));
  int r;
  for (r = 0; r < ROWS; r++)
    board[r] = calloc(COLS + 1, sizeof(char));
  return board;
}

//Frees the board to prevent memory leakage
void free_board(char **board) {
  int i = 0;
  for (; i < ROWS + 1; i++)
    free(board[i]);
  free(board);
}


//Print and format the board to be readable
void print_board(char **board) {
  int i, j;

  //top layer
  printf("┌");
  for (i = 0; i < COLS; i++)
    printf("─┬");
  printf("\b┐\n");


  for (i = ROWS - 1; i >= 0; i--) {
    for (j = 0; j < COLS; j++) {
      // print board contents
      printf("│%c", (board[i][j]? board[i][j] : ' '));
    }

    //space out areas between rows
    printf("│\n");
    printf("├");
    for(; j > 0; j--)
      printf("─┼");
    printf("\b┤\n");
  }

  //print nums at bottom
  for (i = 0; i < COLS; i++)
    printf(" %d", i+1);
  printf("\n");
}

//Does 1 player's turn
void player_move(char** board, int *turn) {
  //Figure out which player is moving
  char letter = ((*turn)%2== 0? 'X' : 'O');
  printf("It's %c's turn.\n", letter);

  char turn_complete = 0;
  while (!turn_complete) { //Keep prompting the user until they give a valid move
    int c = get_user_input() - 1;
    int r = 0;

    //Check if the row is full, if not fill the next available space
    while (r < ROWS)
      if (board[r][c])
        r++;
      else {
        board[r][c] = letter;
        r = ROWS;
        turn_complete = 1;
      }
    if (!turn_complete) printf("That collumn is full!\n");
  }

  (*turn)++;
}

//Checks to see if a player has already won the game
char check_victory(char **board) {
  int r, c;

  //check horizontals
  for (r = 0; r < ROWS; r++) {
    for (c = 0; c <= COLS - 4; c++)
      if (board[r][c] && board[r][c] == board[r][c+1] && board[r][c] == board[r][c+2] && board[r][c] == board[r][c+3]) return board[r][c];
  }

  //check verticals
  for (c = 0; c < COLS; c++) {
    for (r = 0; r <= ROWS - 4; r++)
      if (board[r][c] && board[r][c] == board[r+1][c] && board[r][c] == board[r+2][c] && board[r][c] == board[r+3][c]) return board[r][c];
  }

  //check diagonals
  for (r = 0; r <= ROWS - 4; r++) {
    for (c = 0; c <= COLS - 4; c++) {
      if (board[r][c] && board[r][c] == board[r+1][c+1] && board[r][c] == board[r+2][c+2] && board[r][c] == board[r+3][c+3]) return board[r][c];
    }
    for (c = 3; c < COLS; c++)
      if (board[r][c] && board[r][c] == board[r+1][c-1] && board[r][c] == board[r+2][c-2] && board[r][c] == board[r+3][c-3]) return board[r][c];
  }

  return 0;
}

int main() {
  printf("Hello, and welcome to connect 4\n");

  //Initialize variables
  char ** board = init_board();
  int turn = 0;
  char finished = 0;

  while (!finished) {
    //Show the user what they need to play the game
    print_board(board);
    player_move(board, &turn);

    //Check for gameover
    char win = check_victory(board);
    if (win) {
      print_board(board);
      printf("Game over, the winner is %c.\n", win);
      finished = 1;
    } else if (turn == ROWS * COLS) {
      print_board(board);
      printf("Game over, it's a tie.\n");
      finished = 1;
    }

  }
  free_board(board); //prevent mmory leaks

  return 0;
}
