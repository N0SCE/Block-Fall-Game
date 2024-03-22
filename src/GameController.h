#ifndef PA2_GAMECONTROLLER_H
#define PA2_GAMECONTROLLER_H

#include "BlockFall.h"

using namespace std;

class GameController {
public:
    bool play(BlockFall &game, const string &commands_file); // Function that implements the gameplay
    
    bool enter_grid(BlockFall& game, int entering_position);
    void print_grid(BlockFall& game, Block* block);
    void rotate_right(BlockFall& game, Block* block);
    void rotate_left(BlockFall& game, Block* block);
    void move_right(BlockFall& game, Block* block);
    void move_left(BlockFall& game, Block* block);
    void drop(BlockFall& game, Block* block);
    void gravity_switch(BlockFall& game);
    bool can_move(BlockFall& game, Block* block, int position_to_go);
    void check_powerup(BlockFall& game);
    void clear_rows(BlockFall& game);
    void clear_grid(BlockFall& game);
    int point = 0;
    int position_to_drop = 0;
    int entering_blocks = 0;
    bool gravity_mode = false;
};


#endif //PA2_GAMECONTROLLER_H
