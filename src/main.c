#include "../include/chess.h"

int main(){
    
    board *b = create_board();
    init_board_def(b);

    print_board(b);

    piece_c c = WHITE;
    piece_t t = PAWN;
    
    int pos_1 = get_position(b,c,t, 1);   
    int pos_2 = get_position(b, c, t, 2);
    printf("pos_1: %d, pos_2: %d\n", pos_1, pos_2);
    
    //if(move(b, c, t, pos_1, 48)){printf("piece taken\n");} else{printf("no piece there\n");}

    
    link **moveList = pawn_move_range(b, c, pos_2);

    /*pos_1 = get_position(b,c,t, 1);   
    pos_2 = get_position(b, c, t, 2);
    
    printf(" pos_1: %d, pos_2: %d\n", pos_1, pos_2);*/
    

    print_moves(moveList, PAWN_DIRS);
    free_moveList(moveList, PAWN_DIRS);

    free_board(b);

    /*uint64_t *test;
    *test = 0xFF00;

    int last_pos = 63;
    int first_pos = 0;*/

    return 0;
}
