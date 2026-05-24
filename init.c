#include "init.h"

board *create_board(){
    return (board *)malloc(sizeof(board));
}

void free_board(board *b){
    free(b);
}

void init_board_def(board *b){
    
    b->white[PAWN] = 0x000000000000FF00;
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
        printf("%d ->", p->pos);
    }
    printf("%d\n", p->pos);
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

#define UL 0
#define UP 1
#define UR 2
#define UL_off 7
#define UP_off 8
#define UR_off 9
#define FIRST_COL 0
#define LAST_COL 7 
link *pawn_move_range(board *b, piece_c color, int pos, int dir){
    
    uint64_t enemy_mask = colorMask(b, !color); 
    uint64_t ally_mask = colorMask(b, color); 
    
    int color_off= 1;
 
    if(color == BLACK){
        color_off= -1;
    }
    int data;
    link *head = NULL;
    
    
    switch(dir){
        case UL:
            if (pos % DIM == FIRST_COL)
                return head;

            if (enemy_mask >> (data = (pos + UL_off * color_off)) & 1){
                head = create_link(data);
                return head;
            }
            else{
            }
            break;
        case UR:
            if (pos % DIM == LAST_COL) 
                return head;

            if (enemy_mask >> (data = (pos + UR_off * color_off)) & 1){
                head = create_link(data);
                return head;
            }
            break;
        case UP:
            if (ally_mask >> (data = (pos + UP_off * color_off)) & 1 ){
                return head;
            }
            else {
                head = create_link(data);
            }
            
            int first_move = 0;
            switch(color){
                case WHITE:
                    if(pos / DIM == 1) first_move = 1;
                    break;
                case BLACK:
                    if(pos / DIM == 6) first_move = 1;
                    break;
            }
            if (first_move){
                if(ally_mask >> (data = (pos + 2 * (UP_off * color_off))) & 1){
                    return head;
                }
                head->next = create_link(data);
            }
            break;
    }
    return head;
}
link *knight_move_range(board *, piece_c, int, int){
    
}
#define 
link *bishop_move_range(board *, piece_c, int, int){
    
    uint64_t enemy_mask = colorMask(b, !color); 
    uint64_t ally_mask = colorMask(b, color); 
    


}
link *rook_move_range(board *, piece_c, int, int);

#define PAWN_DIRS 3
void find_pawn_moves(board *b, piece_c color){
    int pos, n = 1;
    while((pos = get_position(b, color, PAWN, n++)) != MISSING){
        link *moves[PAWN_DIRS];
        for(int i = UL; i <= UR; i++){
            moves[i] = pawn_move_range(b, color, pos, i); 
        }
        printf("start pos: %d ... ", pos);
        print_moves(moves, PAWN_DIRS);
        for(int j = UL; j <= UR; j++){
            free_link(moves[j]);
        }

    }
}

void find_knight_moves(board *, piece_c);
void find_bishop_moves(board *, piece_c);
void find_rook_moves(board *, piece_c);
void find_queen_moves(board *, piece_c);
void find_king_moves(board *, piece_c);

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
    
    /*board *b = create_board();
    init_board_def(b);

    piece_c c = WHITE;

    find_pawn_moves(b, c);


    free_board(b);*/

    uint64_t *test;
    *test = 0xFF00;

    clear_pos(test, 8);
    printf("test = %" PRIx64 "\n", *test);
    
    update_pos(test, 8);
    printf("test = %" PRIx64 "\n", *test);

    return 0;
}
