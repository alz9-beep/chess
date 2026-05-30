#include "../include/chess.h"

char file_c[8] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h' };

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

void clear_pos(uint64_t *a, int pos){ //mask and position
	*a &= ~(1ULL << pos);
}

void update_pos(uint64_t *a, int pos){
	*a |= (1ULL << pos);
}

int in_bounds(int pos, int dir){
    

    int old_col = pos % DIM;
    

    pos += dir;

    int new_col = pos % DIM;

    
    if(pos > 63 || pos < 0){ 
        //printf("POS ERR\n");
        return 0; 
    } 

    switch(dir){
        case NE: case E: case SE:
            if (new_col < old_col){ 
                //printf("RIGHT BOUND ERR, new_col: %d, old_col: %d\n", new_col, old_col);
                return 0; 
            }
            break;
        case NW: case W: case SW:
            if (new_col > old_col){ 
                //printf("LEFT BOUND ERR, new_col: %d, old_col: %d\n", new_col, old_col);
                return 0; 
            }
    }

    return 1; 

}

void print_moves(link **arr, int dirs){
    for(int i = 0; i < dirs; i++){
        print_links(arr[i]);
    }
}

void print_links(link *head){
    if (head == NULL){ printf("NULL\n");return;}
    link *p = head;
    for(; p->next != NULL; p = p->next){
        pos_to_char( p->pos);
        printf(" -> ");
    }
    pos_to_char( p->pos);
    printf("\n");
}


void pos_to_char(int pos){
    printf("%c%d", file_c[pos % DIM], pos / 8+ 1);
}
