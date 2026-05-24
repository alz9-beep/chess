#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>

#define DIM 8

typedef enum pieceColor { WHITE, BLACK } piece_c;

typedef enum pieceType { PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING } piece_t;

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
void free_link(link *);
void print_moves(link **, int); //int -> number of directions
void print_links(link *);

uint64_t colorMask(board *, piece_c);
int get_position(board *, piece_c, piece_t, int); //int represent iteration
                                                  //
link *pawn_move_range(board *, piece_c, int, int); //returns LinkedList head from b, pos, dir, and dist
link *knight_move_range(board *, piece_c, int, int);
link *bishop_move_range(board *, piece_c, int, int);
link *rook_move_range(board *, piece_c, int, int);

void find_pawn_moves(board *, piece_c);
void find_knight_moves(board *, piece_c);
void find_bishop_moves(board *, piece_c);
void find_rook_moves(board *, piece_c);
void find_queen_moves(board *, piece_c);
void find_king_moves(board *, piece_c);

piece_t piece_in_pos(uint64_t *, int);
void clear_pos(uint64_t *, int);
void update_pos(uint64_t *, int);
int move(board *, piece_c, piece_t, int old_pos, int new_pos); //returns 1 on piece capture

int game_over(board *, piece_c); //1 if game over, opposite color of piece_c wins!





//create board, init
//moves start from player indicated by init (white default)
//game ends when king is taken or resignation
