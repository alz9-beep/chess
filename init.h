#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdbool.h>

#define DIM 8

#define N 8
#define S -8
#define E 1
#define W -1

#define NW 7
#define NE 9
#define SW -9
#define SE -7
#define MSB 0x8000000000000000
#define LSB 1

#define PAWN_DIRS 3
#define PIECE_DIRS 4


typedef enum pieceColor { WHITE, BLACK } piece_c;

typedef enum pieceType { PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING } piece_t;

typedef enum shiftDirection { LEFT, RIGHT } shift_dir;

char file[8] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h' };

typedef struct Chess_Board {
    uint64_t white[6]; 
    uint64_t black[6];
} board;

typedef struct Move_Range {
    int pos;
    struct Move_Range *next;
} link;

board *create_board(); //mallocs board
void free_board(board *);
void init_board_def(board *); //default init, white start
                            
link *create_link(int);
link **create_moveList(int);//creates arr of link* for each dir
void free_link(link *);
void free_moveList(link **, int);
void print_moves(link **, int); //int -> number of directions
void print_links(link *);
void print_board(board *);
void pos_to_char(int);

int in_bounds(int pos, int dir);//checks bound rules

uint64_t colorMask(board *, piece_c);
int get_position(board *, piece_c, piece_t, int); //int represent iteration
                                                  //
link **pawn_move_range(board *, piece_c, int); //returns LinkedList head from b, pos, dir, and dist
link **knight_move_range(board *, piece_c, int);
link **bishop_move_range(board *, piece_c, int);
link **rook_move_range(board *, piece_c, int);

void find_pawn_moves(board *, piece_c);

piece_t piece_in_pos(uint64_t *, int);
void clear_pos(uint64_t *, int);
void update_pos(uint64_t *, int);
int move(board *, piece_c, piece_t, int old_pos, int new_pos); //returns 1 on piece capture
link *range(uint64_t, uint64_t, int pos, int dir, int max);//checks squares in range of pos given direction
void m_shift(uint64_t *, int pos, shift_dir); //shifts mask pos positions in direction shift_dir

int game_over(board *, piece_c); //1 if game over, opposite color of piece_c wins!





//create board, init
//moves start from player indicated by init (white default)
//game ends when king is taken or resignation
