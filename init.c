#include "init.h"

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

link *create_link(int pos){
    link *ret = (link *)malloc(sizeof(link));
    ret->pos = pos;
    ret->next = NULL;
    return ret;
}

link **create_moveList(int num_dirs){
    return (link **)malloc(sizeof(link *) * num_dirs);
}

void free_moveList(link **arr, int num_dirs){
    for(int i = 0; i < num_dirs; i++){
        free_link(arr[i]);
    }
}



void free_link(link *head){
    link *n, *ptr = head;
    while(ptr != NULL){
        n = ptr->next;
        free(ptr); 
        ptr = n;
    }
}

void print_moves(link **arr, int dirs){
    for(int i = 0; i < dirs; i++){
        print_links(arr[i]);
    }
}

void print_links(link *head){
    if (head == NULL){ printf("NULL");return;}
    link *p = head;
    for(; p->next != NULL; p = p->next){
        pos_to_char( p->pos);
        printf("%d", p->pos);
        printf(" -> ");
    }
    pos_to_char( p->pos);
    printf("\n");
}

void print_board(board *b){

    uint64_t *white_mask, *black_mask;

    *white_mask = colorMask(b, WHITE);
    *black_mask = colorMask(b, BLACK);

    for(int i = 0; i < DIM; i++){
        for(int j = 0; j < DIM; j++){
            
            if(*white_mask & 1){ printf(" 1 "); }
            else if(*black_mask & 1){ printf(" 2 "); }
            else { printf(" 0 "); }
            m_shift(white_mask, 1, RIGHT);
            m_shift(black_mask, 1, RIGHT);
        }
        printf("\n");
    }
}

void pos_to_char(int pos){
    printf("%c%d", file[pos % DIM], pos / 8+ 1);
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

#define MISSING -1

int get_position(board *b, piece_c color, piece_t type, int n){

    uint64_t pieceMask; 
    int pos = 0;

    switch(color){
        case WHITE:
           pieceMask = b->white[type]; 
        break;
        case BLACK:
           pieceMask = b->black[type]; 
        break;
    }

    while(pos < 63){
        if(1 & pieceMask){
            n--;
            if (!n)
                return pos;
        }
        pieceMask >>= 1;
        pos++;
    }
    if(pos == 62 && pieceMask){
        return ++pos;
    }
    return MISSING;

}

#define FIRST_COL 0
#define LAST_COL 7 

link **pawn_move_range(board *b, piece_c color, int pos){
    
    uint64_t enemy_mask = colorMask(b, !color); 
    uint64_t ally_mask = colorMask(b, color); 
    
    int color_off= 1;
 
    if(color == BLACK){
        color_off= -1;
    }
    
    return NULL;
}
    
link **knight_move_range(board * b, piece_c color, int pos){
    return NULL;
}

    
link **bishop_move_range(board * b, piece_c color, int pos){
    
    uint64_t enemy_mask = colorMask(b, !color); 
    uint64_t ally_mask = colorMask(b, color);

    int directions[PIECE_DIRS] = { NW, NE, SE, SW };

    link **movelist = create_moveList(PIECE_DIRS);

    for(int i = 0; i < PIECE_DIRS; i++){
       movelist[i] = range(ally_mask, enemy_mask, pos, directions[i], DIM);
    }

    return movelist;
}

int in_bounds(int pos, int dir){

    int old_col = pos % DIM;

    pos += dir;

    int new_col = pos % DIM;
    
    if(pos > 63 || pos < 0){ return 0; } 

    switch(dir){
        case NE: case E: case SE:
            if (new_col < old_col){ return 0; }
            break;
        case NW: case W: case SW:
            if (new_col > old_col){ return 0; }
    }

    return 1; 

}

link *range(uint64_t ally_mask, uint64_t enemy_mask, int pos, int dir, int max){
    
    link *curr, *prev, *head = NULL;

    shift_dir direction;
    uint64_t bit_check;

    switch(dir){
        case N: case NW: case NE: case E:
            direction = RIGHT;
            bit_check = LSB;
        case S: case SE: case SW: case W:
            direction = LEFT;
            bit_check = MSB;
    }

    uint64_t *ally = &ally_mask, *enemy = &enemy_mask;
    
    m_shift(ally, pos, direction); //align to current piece
    m_shift(enemy, pos, direction); 
    
    
   for(int i = 0; i < max; i++){
        
       m_shift(ally, dir, direction); 
       m_shift(enemy, dir, direction); 

       pos += dir;

       if (!in_bounds(pos, dir)){ return head; }

       if(ally_mask & bit_check){ return head; }

       if(enemy_mask & bit_check && i){
           curr = create_link(pos);
           prev->next = curr;
           return head;
       } else if(enemy_mask & bit_check){
           head = create_link(pos);
           return head;
       }
       
       if(i){
            curr = create_link(pos);
            prev->next = curr; 
            prev = curr;
       }else{
           head = create_link(pos);
           prev = head;
       }
   }

    return head;
}
void m_shift(uint64_t *mask, int pos, shift_dir dir){
    switch(dir){
        case LEFT:
            *mask <<= pos;
            break;
        case RIGHT:
            *mask >>= pos;
            break;
    }
}



link **rook_move_range(board *, piece_c, int);


piece_t piece_in_pos(uint64_t *p, int pos){ //takes b->white/black arr as argument
    for(piece_t i = PAWN; i <= KING; i++){
        if((*(p + i)) >> pos & 1){
            return i;
        }
    }
    return -1;
}
void clear_pos(uint64_t *a, int pos){ //mask and position
    uint64_t clear_mask = ~(1 << pos);
    *a &= clear_mask;
}
void update_pos(uint64_t *a, int pos){
    uint64_t update_mask = (1 << pos);
    *a |= update_mask;
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

int main(){
    
    board *b = create_board();
    init_board_def(b);

    print_board(b);

    piece_c c = WHITE;
    int pos = 0x20;
    link **moveList = bishop_move_range(b, c, pos);

    print_moves(moveList, PIECE_DIRS);
    free_moveList(moveList, PIECE_DIRS);

    free_board(b);

    /*uint64_t *test;
    *test = 0xFF00;

    int last_pos = 63;
    int first_pos = 0;


    if(in_bounds(first_pos + 6, E)){
        printf("in bounds\n");
    }else{printf("out of bounds\n");}

    

    m_shift(test, 1, LEFT);
    printf("test = %" PRIx64 "\n", *test);

    m_shift(test, 1, RIGHT);
    printf("test = %" PRIx64 "\n", *test);*/

    return 0;
}
