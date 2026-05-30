#include "../include/chess.h"



//PHYISCAL BOARD MANIPULATION

board *create_board(){
    return (board *)malloc(sizeof(board));
}

void free_board(board *b){
    free(b);
}

void init_board_def(board *b){
    
    b->white[PAWN] = 0x0000000000000000;

    b->black[PAWN] = 0x00FF000000000000;

    b->white[KNIGHT] = 0x0000000000000042;
    b->black[KNIGHT] = 0x4200000000000000;
    
    b->white[BISHOP] = 0x0000000000000024;
    b->black[BISHOP] = 0x2400000000000000;
    
    b->white[ROOK] = 0x0000000000000081;
    b->black[ROOK] = 0x8100000000000000;
    
    b->white[QUEEN] = 0x0000000000000008;
    b->black[QUEEN] = 0x0800000000000000;

    b->white[KING] = 0x0000000000000010;
    b->black[KING] = 0x1000000000000000;

}

void print_board(board *b){

    uint64_t white, black;

    uint64_t *white_mask = &white, *black_mask = &black;


    white = colorMask(b, WHITE);
    black = colorMask(b, BLACK);


    for(int i = 0; i < DIM; i++){
        int buf[8], *bufp = buf;
        for(int j = DIM - 1; j >= 0; j--){
           
            if(*white_mask & MSB){ buf[j] = 1; }
            else if(*black_mask & MSB){ buf[j] = 2; }
            else { buf[j] = 0 ; }
           
            m_shift(white_mask, 1, LEFT);
            m_shift(black_mask, 1, LEFT);

        }
        print_buf(bufp);
    }
}

void print_buf(int *bp){
    for(int i = 0; i < DIM; i++){
        printf(" %d ", bp[i]);
    }
    printf("\n");
}

uint64_t colorMask(board * b, piece_c color){
    
    uint64_t ret = 0;
   
    switch(color){
        case WHITE:
            for(int i = PAWN; i <= KING; i++){
                ret |= b->white[i];
            }
            break;
        case BLACK:
            for(int i = PAWN; i <= KING; i++){
                ret |= b->black[i];
            }
            break;
    }
   
    return ret;
        
}

piece_t piece_in_pos(uint64_t *p, int pos){ //takes b->white/black arr as argument
    for(piece_t i = PAWN; i <= KING; i++){
        if((*(p + i)) >> pos & 1){
            return i;
        }
    }
    return -1;
}

int move(board *b, piece_c color, piece_t type, int pos_o, int pos_n){
    
    uint64_t *ally, *enemy;
    
    switch(color){
        case WHITE:
            ally = b->white;
            enemy = b->black;
            break;
        case BLACK:
            ally = b->black;
            enemy = b->white;
            break;
    }

    clear_pos(ally + type, pos_o);
    update_pos(ally + type, pos_n);

    piece_t pieceType;

    if((pieceType = piece_in_pos(enemy, pos_n)) != -1){
        clear_pos(enemy + pieceType, pos_n);
        return 1;
    }

    return 0;
    
}



