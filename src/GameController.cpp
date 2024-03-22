#include "GameController.h"
#include <fstream>
#include <iostream>


bool GameController::play(BlockFall& game, const string& commands_file){

    // TODO: Implement the gameplay here while reading the commands from the input file given as the 3rd command-line
    //       argument. The return value represents if the gameplay was successful or not: false if game over,
    //       true otherwise.

    bool is_playable = false;
    bool enter_next_block = true;
    //std::ifstream myFile(commands_file);
    std::ifstream myFile;
    myFile.open(commands_file, ios::in);
    
    std::string line;

    if (!myFile.is_open()) {
        std::cerr << "Error opening the file: " << commands_file << std::endl;
        // Output additional error information
        std::perror("File opening failed");
        
    }

    if(myFile.is_open()){


        while (std::getline(myFile, line))
        {
            if(enter_next_block){
                if(enter_grid(game, position_to_drop) == false){
                is_playable = false;
                break; // do smth with that, print error...
                } 
                enter_next_block = false;
            }            



            Block* temp = game.active_rotation; // check if you need to dealloced
            Block* temp_normal = game.initial_block;
            for(int i = 0; i < entering_blocks -1; i++){
                if(temp->next_block != nullptr && temp_normal->next_block != nullptr){
                    temp = temp->next_block;
                    temp_normal = temp_normal->next_block;
                }
            }

            

            if(line == "PRINT_GRID"){
                print_grid(game, temp); // complete
            }else if (line == "ROTATE_RIGHT")
            {
                rotate_right(game, temp);
                
            }else if (line == "ROTATE_LEFT")
            {
                rotate_left(game, temp);
            }else if (line == "MOVE_RIGHT")
            {
                move_right(game, temp);
            }
            else if (line == "MOVE_LEFT")
            {
                move_left(game, temp);
            }else if (line == "DROP")
            {
                drop(game, temp);
                enter_next_block = true;
            }else if (line == "GRAVITY_SWITCH")
            {
                gravity_switch(game);
            }else{
                // do smth, error
            }
            
            
            

            is_playable = true;
        }
        myFile.close();
    }

    
    return is_playable;
}

void GameController::print_grid(BlockFall& game, Block* block){
    std::cout << "Score: " << point  << "\n";
    std::cout << "High Score: 0" << "\n"; // do smth

    for(int i = 0; i < game.grid.size(); i++){
        for(int j = 0; j < game.grid[0].size(); j++){
            if(game.grid[i][j] == 0 &&  j != game.grid[0].size() -1){
                std::cout << "⬛" << " "; 
            }else if(game.grid[i][j] == 0 &&  j == game.grid[0].size() -1){
                std::cout << "⬛"; 
            }else if(game.grid[i][j] == 1 &&  j != game.grid[0].size() -1){
                std::cout << "⬜" << " ";
            }else if(game.grid[i][j] == 1 &&  j == game.grid[0].size() -1){
                std::cout << "⬜";
            }

            std::cout << "\n";

            if(i == game.grid.size() -1){
                std::cout << "\n";
                std::cout << "\n";
            }
        }
    }

}

bool GameController::enter_grid(BlockFall& game, int position_to_drop){
    bool can_enter_grid = true;
    

    
    Block* temp = game.active_rotation; // check if you need to dealloced
    for(int i = 0; i < entering_blocks; i++){
        if(temp->next_block != nullptr){
            temp = temp->next_block;

        }
    }

    int row_size = temp->shape.size();
    int column_size = temp->shape[0].size();

    if(position_to_drop + column_size > game.grid[0].size()){
        can_enter_grid = false;
        return can_enter_grid;
    }


    for(int i = 0; i < row_size; i++){
        for(int j = 0; j < column_size; j++){
            for(int k = 0; k < row_size - i; k++){
                if(game.grid[k][column_size - 1  - j + position_to_drop] != 0 && temp->shape[row_size - 1  - i][column_size - 1 - j] != 0){ // check , add and 
                    can_enter_grid = false;
                    break;
                }
            }

            if(can_enter_grid == false){
            break;
            }
            
        }

        if(can_enter_grid == false){
            break;
        }
    }


    
    
    entering_blocks++;
    return can_enter_grid;
}


void GameController::rotate_right(BlockFall& game, Block* block){
    if (block == nullptr) {
        return;
    }

    
    if(can_move(game, block->right_rotation, 0)){

        for(int i = 0; i < block->shape[0].size(); i++){
            for(int j = 0; j < block->shape.size(); j++){
                game.grid[i][j + position_to_drop] == 0;
            }
        }

        block = block->right_rotation;

        for(int i = 0; i < block->shape[0].size(); i++){
            for(int j = 0; j < block->shape.size(); j++){
                game.grid[i][j + position_to_drop] == 1;
            }
        }
    }
    
    

}

void GameController::rotate_left(BlockFall& game, Block* block){
    if (block == nullptr) {
        return;
    }
    
    if(can_move(game, block->right_rotation, 0)){

        for(int i = 0; i < block->shape[0].size(); i++){
            for(int j = 0; j < block->shape.size(); j++){
                game.grid[i][j + position_to_drop] == 0;
            }
        }

        block = block->left_rotation;

        for(int i = 0; i < block->shape[0].size(); i++){
            for(int j = 0; j < block->shape.size(); j++){
                game.grid[i][j + position_to_drop] == 1;
            }
        }
    }
}

void GameController::move_right(BlockFall& game, Block* block){

    

    bool can_move = true;
    can_move = GameController::can_move(game, block, 1);
    int row_size = block->shape.size();
    int column_size = block->shape[0].size();

    for(int i = 0; i < block->shape[0].size(); i++){
        for(int j = 0; j < block->shape.size(); j++){ 
            if(game.grid.size() > j && game.grid[0].size() > column_size - i +1 + position_to_drop){ // check for bound errors

                if(game.grid[j][column_size - i +1 + position_to_drop] != 0 && block->shape[j][column_size - i -1] != 0){ // change this as their drop positions
                    can_move = false;
                    break;
                }
            }else{
                can_move = false;
            }

            if(can_move == false){
                break;
            }
            
        }

        if(can_move == false){
            break;
        }
    }

    if(can_move){
        position_to_drop++;
        for(int i = 0; i < block->shape[0].size(); i++){
            for(int j = 0; j < block->shape.size(); j++){
                game.grid[i][j + position_to_drop -1] = 0;
            }
        }

        for(int i = 0; i < block->shape[0].size(); i++){
            for(int j = 0; j < block->shape.size(); j++){
                game.grid[i][j + position_to_drop] = 1;
            }
        }
    }

    
}


void GameController::move_left(BlockFall& game, Block* block){    
    bool can_move = true;
    can_move = GameController::can_move(game, block, -1);
    if (!can_move) {
        return;
    }
    int row_size = block->shape.size();
    int column_size = block->shape[0].size();

    for(int i = 0; i < block->shape[0].size(); i++){
        for(int j = 0; j < block->shape.size(); j++){ 
            if(game.grid.size() >= j && game.grid[0].size() >= row_size - i -1 + position_to_drop){ // check for bound errors

                if(game.grid[j][column_size - i -1 + position_to_drop] != 0 && block->shape[j][column_size - i -1] != 0){ // change this as their drop positions
                    can_move = false;
                    break;
                }
            }else{
                can_move = false;
            }

            if(can_move == false){
                break;
            }
            
        }

        if(can_move == false){
            break;
        }
    }

    if(can_move){
        position_to_drop--;
        for(int i = 0; i < block->shape[0].size(); i++){
            for(int j = 0; j < block->shape.size(); j++){
                game.grid[i][j + position_to_drop + 1] = 0;
            }
        }

        for(int i = 0; i < block->shape[0].size(); i++){
            for(int j = 0; j < block->shape.size(); j++){
                game.grid[i][j + position_to_drop] = 1;
            }
        }
    }
}

void GameController::drop(BlockFall& game, Block* block){
    int falling_distance = 0;
    bool is_there_block = false;
    
    int total_block = 0;
    
    if(game.gravity_mode_on == false){ // consider position to drop
        for(int i = 0; i < block->shape.size(); i++){
            for(int j = 0; j < block->shape[0].size(); j++){
                if(block->shape[i][j] == 1){
                    for(int k = i + 1; k < block->shape.size(); k++){
                        if(block->shape[k][j] == 1){
                            break;
                        }else if(k == block->shape.size() -1){
                            is_there_block = false;
                            for(int l = i + 1; l < game.rows; l++){
                                if (game.grid[l][j + position_to_drop] == 1)
                                {
                                    if(falling_distance == 0){
                                        falling_distance = l - i;                                        
                                    }else if(l - i < falling_distance){
                                        falling_distance = l -i;                                        
                                    }
                                    is_there_block = true;
                                    break;
                                }else if(is_there_block == false && l == game.rows -1){
                                    if(falling_distance == 0){
                                        falling_distance = l - i;                                        
                                    }else if(l - i < falling_distance){
                                        falling_distance = l - i + 1;                                        
                                    }
                                }
                                
                            }
                        }
                    }
                }
            }
        }


        // make fall the blocks, use falling (distance - 1)
        total_block = 0;
        for(int i = 0; i < block->shape.size(); i++){
            for(int j = 0; j < block->shape[0].size(); j++){
                if(block->shape[i][j] == 1){
                    game.grid[i][j + position_to_drop] = 0; // check
                    if (falling_distance != 0 && i != 0) {
                        game.grid[i + falling_distance - 1][j + position_to_drop] = 1;
                        total_block++;
                    }
                    
                }
                
            }
            
        }
        point += total_block * (falling_distance -1);
        check_powerup(game);
        clear_rows(game);

    }else{ 
        int min_falling_distance = 0;
        falling_distance = 0;
        total_block = 0;
        for(int i = 0; i < block->shape.size(); i++){
            for(int j = 0; j < block->shape[0].size(); j++){
                if(block->shape[block->shape.size() - 1 - i][block->shape[0].size() - 1 - j] == 1){
                    total_block++;
                    for(int l = block->shape.size() - 1 - i + 1; l < game.rows; l++){ 
                        if (game.grid[l][j + position_to_drop] == 1){
                            falling_distance = l - i;
                            game.grid[i][j + position_to_drop] = 0;
                            game.grid[i + falling_distance -1][j + position_to_drop] = 1;
                            if(min_falling_distance == 0){
                                min_falling_distance = falling_distance;                                        
                            }else if(falling_distance < min_falling_distance){
                                min_falling_distance = falling_distance;                                        
                            }
                            break;
                        }

                        if(l == game.rows -1){
                            if(falling_distance == 0){
                                falling_distance = l - i;                                        
                            }else if(l - i < falling_distance){
                                falling_distance = l - i + 1;                                        
                            }

                            if(min_falling_distance == 0){
                                min_falling_distance = falling_distance;                                        
                            }else if(falling_distance < min_falling_distance){
                                min_falling_distance = falling_distance;                                        
                            }
                            game.grid[i][j + position_to_drop] = 0;
                            game.grid[i + falling_distance -1][j + position_to_drop] = 1;
                        }
                    }
                }
                
            }
            
        }

        check_powerup(game); // you can change the place of it 
        clear_rows(game);
        point += total_block * (falling_distance -1);
    }
    



    // do other stuff, clear powerups...
}

void GameController::gravity_switch(BlockFall& game){
    //gravity_mode = !gravity_mode;
    game.gravity_mode_on = !game.gravity_mode_on;
}


bool GameController::can_move(BlockFall& game, Block* block, int position_to_go){
    bool can_move = true;
    if(block->shape[0].size() + position_to_go + position_to_drop > game.grid[0].size() || position_to_drop + position_to_go < 0){
        can_move = false;
        return can_move;
    }

    //check grid
    for(int i = 0; i < block->shape.size(); i++){
        for(int j = 0; j < block->shape[0].size(); j++){
            if(block->shape[i][j] == 1 && game.grid[i][j + position_to_drop + position_to_go] == 1){
                can_move = false;
                break;
            }
        }

        if(!can_move){
            break;
        }
    }
    
    return can_move;
}

void GameController::check_powerup(BlockFall& game){
    bool is_equal = true;
    if (game.power_up.size() == 0) {
        return;
    }
    for(int i = 0; i < game.grid.size() - game.power_up.size() + 1; i++){
        for(int j = 0; j < game.grid[0].size() - game.power_up[0].size() + 1; j++){
            for(int k = 0; k <game.power_up.size(); k++){
                for(int l = 0; l < game.power_up[0].size(); l++){
                    if(game.grid[i + k][j + l] != game.power_up[k][l]){
                        is_equal = false;
                        break;
                    }
                }
            }
        }
    }

    if(is_equal){
        clear_grid(game);
    }


}


void GameController::clear_rows(BlockFall& game){
    bool is_row_full = true;
    while(is_row_full){
        for(int i = game.grid.size() -1; i < game.grid.size(); i++){
            for(int j = 0; j < game.grid[0].size(); j++){
                if(game.grid[i][j] == 0){
                    is_row_full = false;
                    break;
                }
            }

            if(is_row_full == false){
                break;
            }
        }

        if(is_row_full){
            for(int i = game.grid.size() -1; i < game.grid.size(); i++){
                for(int j = 0; j < game.grid[0].size(); j++){
                    point++;
                    game.grid[i][j] = 0;
                }
            }
        }
    }
    


}

void GameController::clear_grid(BlockFall& game){
    int cleared_cells = 0;
    for(int i = 0; i < game.grid.size(); i++){
        for(int j = 0; j < game.grid[0].size(); j++){
            if(game.grid[i][j] == 1){
                cleared_cells++;
                game.grid[i][j] = 0;
            }
        }
    }

    point += 1000 + cleared_cells;
}