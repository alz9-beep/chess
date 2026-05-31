#ifndef CHESS_H
#define CHESS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

// Constants and Directions
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
#define SPEC_DIRS 8

// Enums and Structs
typedef enum { WHITE, BLACK } piece_c;
typedef enum { PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING } piece_t;
typedef enum { LEFT, RIGHT } shift_dir;

typedef struct {
    uint64_t white[6];
    uint64_t black[6];
} board;

typedef struct Move_Range {
    int pos;
    struct Move_Range *next;
} link;

// Prototypes: Board Operations (board.c)
board *create_board();
void free_board(board *b);
void init_board_def(board *b);
void print_board(board *b);
void print_buf(int *);
int move(board *b, piece_c color, piece_t type, int pos_o, int pos_n);
uint64_t colorMask(board *b, piece_c color);
piece_t piece_in_pos(uint64_t *p, int pos);

// Prototypes: Move Generation (movegen.c)
link *create_link(int pos);
link **create_moveList(int num_dirs);
void free_link(link *head);
void free_moveList(link **arr, int num_dirs);
link *range(uint64_t ally_mask, uint64_t enemy_mask, int pos, int dir, int max);
link **pawn_move_range(board *b, piece_c color, int pos);
link **rook_move_range(board *b, piece_c color, int pos);
link **bishop_move_range(board *b, piece_c color, int pos);
int get_position(board *b, piece_c color, piece_t type, int n);
void trim_pawn_moves(link **, int, uint64_t, int); //checks if diagonals are actual captures, trimming invalid

// Prototypes: Utilities (utils.c)
void m_shift(uint64_t *mask, int pos, shift_dir dir);
int in_bounds(int pos, int dir);
int first_pawn_move(int, piece_c);
void pos_to_char(int pos);
void print_moves(link **arr, int dirs);
void print_links(link *head);
void clear_pos(uint64_t *a, int pos);
void update_pos(uint64_t *a, int pos);

#endif
