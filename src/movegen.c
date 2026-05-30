#include "../include/chess.h"

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

link **create_moveList(int num_dirs){
    return (link **)malloc(sizeof(link *) * num_dirs);
}

void free_moveList(link **arr, int num_dirs){
    for(int i = 0; i < num_dirs; i++){
        free_link(arr[i]);
    }
    free(arr);
}

link *range(uint64_t ally_mask, uint64_t enemy_mask, int pos, int dir, int max){
    
    link *curr, *prev, *head = NULL;

    shift_dir direction;
    uint64_t bit_check;

    switch(dir){
        case N: case NW: case NE: case E:
            direction = RIGHT;
            bit_check = LSB;
            break;
        case S: case SE: case SW: case W:
            direction = LEFT;
            bit_check = MSB;
    }


    uint64_t *ally = &ally_mask, *enemy = &enemy_mask;
    
    m_shift(ally, pos, direction); //align to current piece
    m_shift(enemy, pos, direction); 

    int i;

    
   for(i = 0; i < max; i++){
        
       m_shift(ally, dir, direction); 
       m_shift(enemy, dir, direction); 


       if (!in_bounds(pos, dir)){ 
           //printf("out of bounds\n");
           return head; 
       }
       
       pos += dir;

       

       if(enemy_mask & bit_check && i){
           curr = create_link(pos);
           prev->next = curr;
           //printf("enemy piece, capture and end\n");
           return head;
       } else if(enemy_mask & bit_check){
           head = create_link(pos);
           //printf("enemy piece in square\n");
           return head;
       } else if(ally_mask & bit_check){ 
           //printf("ally blocking\n");
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

link **pawn_move_range(board *b, piece_c color, int pos){
    
    uint64_t enemy_mask = colorMask(b, !color); 
    uint64_t ally_mask = colorMask(b, color); 

    int forward, captureLeft, captureRight;

    switch(color){
        case WHITE:
            forward = N;
            captureLeft = NW;
            captureRight = NE;
            break;
        default:
            forward = S;
            captureLeft = SE;
            captureRight = SW;
    }
    int directions[PAWN_DIRS] = { forward, captureLeft, captureRight };

    link **movelist = create_moveList(PAWN_DIRS);

    for(int i = 0; i < PAWN_DIRS; i++){
        movelist[i] = range(ally_mask, enemy_mask, pos, directions[i], 2);
    }
    
    return movelist;
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

link **rook_move_range(board *b, piece_c color, int pos){
    
    uint64_t enemy_mask = colorMask(b, !color); 
    uint64_t ally_mask = colorMask(b, color);

    int directions[PIECE_DIRS] = { N, E, S, W };

    link **movelist = create_moveList(PIECE_DIRS);

    for(int i = 0; i < PIECE_DIRS; i++){
       movelist[i] = range(ally_mask, enemy_mask, pos, directions[i], DIM);
    }

    return movelist;

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


