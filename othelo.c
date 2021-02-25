#include<stdio.h>
#include<string.h>
#define BOARD_SIZE 8
#define HISTORY_SIZE 100
int coordinate_to_int;
int x, y, number_stones, pass = 0, flip_possible, location, number_moves = 0;
int hist = 0;
int p1, p2;
char is_player_first[];
int stones_player1, stones_player2;
int number_stones_obtained =0;
int player_id, num_stones[8] = {0,0,0,0,0,0,0,0};
int history[HISTORY_SIZE];
char char_x[] = "", char_y[] = "", is_player_first[] = "";
FILE *fp;

void print_board(int board[][BOARD_SIZE]);
void init_board(int board[][BOARD_SIZE]);
int count_stone(int player_id, int board[][BOARD_SIZE]);
void place_stone(int player_id, int x, int y, int board[][BOARD_SIZE]);
int num_obtained_stone(int player_id, int x, int y, int board[][BOARD_SIZE]);
int asci_to_int(char string[]);
int input_place(int player_id, int board[][BOARD_SIZE]);
int computer(int player_id, int board[][BOARD_SIZE]);
void update_history(int location, int history[]);
void save_history(int history[]);
int flip_stones(int n, int x, int y, int board[][BOARD_SIZE]);
int validate_coordinate(char coordinate[]);
int function(int player_id,int x, int y, int board[][BOARD_SIZE]);

void print_board(int board[][BOARD_SIZE]){
  printf("Score: P1 %d - P2 %d\n", count_stone(1,board), count_stone(-1,board));
  printf("  0 1 2 3 4 5 6 7\n");
  int i, j;
  for (i = 0; i < BOARD_SIZE; i++) {
    printf("%d ", i);
    for (j = 0; j < BOARD_SIZE; j++) {
      if(board[i][j] == 1) printf("o ");
      else if(board[i][j] == -1) printf("x ");
      else printf(". ");
    }
    printf("\n");
  }
}
void init_board(int board[][BOARD_SIZE]){
  int i, j;
  for (i = 0; i < BOARD_SIZE; i++) {
    for (j = 0; j < BOARD_SIZE; j++) {
      board[i][j] = 0;
    }
  }
  board[3][3] = 1;
  board[3][4] = -1;
  board[4][3] = -1;
  board[4][4] = 1;
  print_board(board);
}
int count_stone(int player_id, int board[][BOARD_SIZE]){
  number_stones = 0;
  int i, j;
  for (i = 0; i < BOARD_SIZE; i++) {
    for (j = 0; j < BOARD_SIZE; j++) {
      if(board[i][j] == player_id) number_stones++;
    }
  }
  return number_stones;
}
void place_stone(int player_id, int x, int y, int board[][BOARD_SIZE]){
  flip_possible = 0;
  if(board[x][y] == 0){
    board[x][y] = player_id;
    flip_stones(1,y,x,board);
  }
}
int num_obtained_stone(int player_id, int x, int y, int board[][BOARD_SIZE]){
  int temp = board[x][y];
  int i;
  number_stones_obtained = 0;
  if(board[x][y] == 0){
    board[x][y] = player_id;
    flip_stones(0,y,x,board);
  }
  for (i = 0; i < 8; i++) {
    number_stones_obtained = number_stones_obtained + num_stones[i];
    num_stones[i] = 0;
  }
  board[x][y] = temp;
  return number_stones_obtained;
}
int asci_to_int(char string[]){
  int i = 0, num = 0, is_not_int = 0;
  while (string[i]!='\0') {
    if(string[i] >= '0' && string[i] <= '9') num = 10*num + (string[i] - '0');
    else {is_not_int++;break;}
    i++;
  }
  if(is_not_int == 0) return num;
  else return -1;
}
int input_place(int player_id, int board[][BOARD_SIZE]){
  printf("Insert y: ");
  scanf("%s", char_x);
  x = validate_coordinate(char_x);
  if(x == -1) return location = -1;
  else{
    printf("Insert x: ");
    scanf("%s", char_y);
    y = validate_coordinate(char_y);
    if(y == -1) return location = -1;
    else{
      if(num_obtained_stone(player_id,x,y,board) == 0 ) {
        printf("You cannot place a stone in (%d,%d)\n",x,y);
        input_place(player_id, board);
      }
      place_stone(player_id,x,y,board);
      return location = x*BOARD_SIZE+y;
    }
  }
}
int computer(int player_id, int board[][BOARD_SIZE]){
  int max_num_flips = 0;
  int max[2] = {0,0};
  int i,j;
  int temp;
  for (i = 0; i < 8; i++) {
    for (j = 0; j < 8; j++) {
      if (board[i][j] == 0){
        temp = num_obtained_stone(player_id,j,i,board);
        //if(temp > max_num_flips){
        if(temp > 0){
          max_num_flips = temp;
          max[0] = i;
          max[1] = j;
        }
      }
    }
  }
  if(max_num_flips == 0) return location = -1;
  else{
    place_stone(player_id,max[1],max[0],board);
    return location = max[1]*BOARD_SIZE+max[0];
  }
}
void update_history(int location, int history[]){
  if(hist == 0){
    int i;
    for (i = 0; i < HISTORY_SIZE; i++) {
      history[i] = -2;
    }
    hist++;
  }
  history[number_moves] = location;
}
void save_history(int history[]){
  int i;
  fp = fopen("history.txt","w");
  for (i = 0; i < number_moves; i++) {
    if(i%2 == 0){
      if(history[i] == -1) fprintf(fp, "PLAYER 1 %2d %2d\n", -1, -1);
      else fprintf(fp, "PLAYER 1 %2d %2d\n", history[i]/8,history[i]%8);
    }
    else{
      if(history[i] == -1) fprintf(fp, "PLAYER 2 %2d %2d\n", -1, -1);
      else fprintf(fp, "PLAYER 2 %2d %2d\n", history[i]/8,history[i]%8);
    }
  }
  fclose(fp);
}

int minimum(int x, int y){
  if(x<=y) return x;
  else return y;
}
int flip_up(int n, int x, int y, int board[][BOARD_SIZE]){
  int i,j;
  if(y > 0){
    for (i = y-1; i > -1; i--) {
      if(board[i][x] == 0 || board[y-1][x] == board[y][x])break;
      else if(board[i][x] == board[y][x]){
        flip_possible++;
        for (j = y-1; j > i; j--) {
          if(n == 1) board[j][x] = board[y][x];
          else num_stones[0]++;
        }
        break;
      }
    }
  }
  return 0;
}
int flip_down(int n, int x, int y, int board[][BOARD_SIZE]){
  int i, j;
  if(y < 7){
    for (i = y+1; i < BOARD_SIZE; i++) {
      if(board[i][x] == 0 || board[y+1][x] == board[y][x])break;
      else if(board[i][x] == board[y][x]){
        flip_possible++;
        for (j = y+1; j < i; j++) {
          if(n == 1) board[j][x] = board[y][x];
          else num_stones[1]++;
        }
        break;
      }
    }
  }
  return 0;
}
int flip_right(int n, int x, int y, int board[][BOARD_SIZE]){
  int i, j;
  if(x < 7){
    for (i = x+1; i < BOARD_SIZE; i++) {
      if(board[y][i] == 0 || board[y][x+1] == board[y][x])break;
      else if(board[y][i] == board[y][x]){
        flip_possible++;
        for (j = x+1; j < i; j++) {
          if(n == 1)board[y][j] = board[y][x];
          else num_stones[2]++;
        }
        break;
      }
    }
  }
  return 0;
}
int flip_left(int n, int x, int y, int board[][BOARD_SIZE]){
  int i, j;
  if(x > 0){
    for (i = x-1; i > -1; i--) {
      if(board[y][i] == 0 || board[y][x-1] == board[y][x])break;
      else if(board[y][i] == board[y][x]){
        flip_possible++;
        for (j = x-1; j > i; j--) {
          if(n == 1) board[y][j] = board[y][x];
          else num_stones[3]++;
        }
        break;
      }
    }
  }
  return 0;
}
int flip_upper_right(int n, int x, int y, int board[][BOARD_SIZE]){
  int i, j;
  if(y > 0 && x < 7){
    int min = minimum(BOARD_SIZE-1-x,y);
    for (i = 1; i < min+1; i++) {
      if(board[y-i][x+i] == 0 || board[y-1][x+1] == board[y][x])break;
      else if(board[y-i][x+i] == board[y][x]){
        flip_possible++;
        for (j = 1; j < i; j++) {
          if(n ==1) board[y-j][x+j] = board[y][x];
          else num_stones[4]++;
        }
        break;
      }
    }
  }
  return 0;
}
int flip_upper_left(int n, int x, int y, int board[][BOARD_SIZE]){
  int i, j;
  if(y > 0 && x > 0){
    int min = minimum(x,y);
    for (i = 1; i < min+1; i++) {
      if(board[y-i][x-i] == 0 || board[y-1][x-1] == board[y][x]) break;
      else if(board[y-i][x-i] == board[y][x]){
        flip_possible++;
        for (j = 1; j < i; j++) {
          if(n == 1) board[y-j][x-j] = board[y][x];
          else num_stones[5]++;
        }
        break;
      }
    }
  }
  return 0;
}
int flip_bottom_right(int n, int x, int y, int board[][BOARD_SIZE]){
  int i, j;
  if(y < 7 && x <7){
    int min = minimum(BOARD_SIZE-1-x,BOARD_SIZE-1-y);
    for (i = 1; i < min+1; i++) {
      if(board[y+i][x+i] == 0 || board[y+1][x+1] == board[y][x])break;
      else if(board[y+i][x+i] == board[y][x]){
        flip_possible++;
        for (j = 1; j < i; j++) {
          if(n == 1) board[y+j][x+j] = board[y][x];
          else num_stones[6]++;
        }
        break;
      }
    }
  }
  return 0;
}
int flip_bottom_left(int n, int x, int y, int board[][BOARD_SIZE]){
  int i, j;
  if(y < 7 && x > 0){
    int min = minimum(x,BOARD_SIZE-1-y);
    for (i = 1; i < min+1; i++) {
      if(board[y+i][x-i] == 0 || board[y+1][x-1] == board[y][x])break;
      else if(board[y+i][x-i] == board[y][x]){
        flip_possible++;
        for (j = 1; j < i; j++) {
          if(n == 1) board[y+j][x-j] = board[y][x];
          else num_stones[7]++;
        }
        break;
      }
    }
  }
  return 0;
}
int flip_stones(int n, int x, int y, int board[][BOARD_SIZE]){
  flip_up(n,x,y,board);
  flip_down(n,x,y,board);
  flip_right(n,x,y,board);
  flip_left(n,x,y,board);
  flip_upper_right(n,x,y,board);
  flip_upper_left(n,x,y,board);
  flip_bottom_right(n,x,y,board);
  flip_bottom_left(n,x,y,board);
  return 0;
}
int validate_coordinate(char coordinate[]){
  if(strcmp(coordinate,"PASS") == 0) return -1;
  else{
    int coordinate_to_int = asci_to_int(coordinate);
    if (coordinate_to_int < 0 || coordinate_to_int > 7) {
      printf("Invalid coordinate\n");
      printf("Insert coordinate: ");
      scanf("%s", coordinate);
      validate_coordinate(coordinate);
    }
    return coordinate_to_int;
  }
}
int game_over(int board[][BOARD_SIZE]){
  int i, j;
  p1 = 0, p2 = 0;
  for (i = 0; i < BOARD_SIZE; i++) {
    for (int j = 0; j < BOARD_SIZE; j++) {
      if(board[i][j] == 1) p1 = p1 + num_obtained_stone(1,j,i,board);
      else p2 = p2 + num_obtained_stone(-1,j,i,board);
    }
  }
  if(p1 == 0 && p2 == 0) return 1;
  else return 0;
}
int ask_is_player_first(){
  printf("Is player first? (y/n) ");
  scanf("%s", is_player_first);
  while (strcmp(is_player_first,"y") != 0 && strcmp(is_player_first,"n") != 0) {
    printf("Is player first? (y/n) ");
    scanf("%s", is_player_first);
  }
  if(strcmp(is_player_first,"y") == 0) return 1;
  else return 0;
}

int main(){
  int BOARD[BOARD_SIZE][BOARD_SIZE];
  int player_first;
  init_board(BOARD);
  player_first = ask_is_player_first();
  while(game_over(BOARD) == 0){
    if(player_first == 1){
      printf("--PLAYER 1\n");
      input_place(1,BOARD);
      if(x!=-1 && y!=-1) place_stone(1,x,y,BOARD);
      update_history(location, history);
      number_moves++;
      computer(-1,BOARD);
      update_history(location, history);
      number_moves++;
      print_board(BOARD);
    }
    else{
      if(number_moves == 0){
        computer(1,BOARD);
        update_history(location, history);
        number_moves++;
        print_board(BOARD);
      }
      if(number_moves > 0){
        printf("--PLAYER 2\n");
        input_place(-1,BOARD);
        if(x!=-1 && y!=-1) place_stone(-1,x,y,BOARD);
        update_history(location, history);
        number_moves++;
        computer(1,BOARD);
        update_history(location, history);
        number_moves++;
        print_board(BOARD);
      }
    }
  }
  stones_player1 = count_stone(1,BOARD);
  stones_player2 = count_stone(-1,BOARD);
  if(stones_player1 > stones_player2) printf("PLAYER 1 WINS\n");
  else if(stones_player2 > stones_player1) printf("PLAYER 2 WINS\n");
  else printf("IT IS A DRAW\n");
  save_history(history);
}
