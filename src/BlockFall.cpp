#include "BlockFall.h"
#include <fstream>
#include <string>

BlockFall::BlockFall(string grid_file_name, string blocks_file_name, bool gravity_mode_on, const string &leaderboard_file_name, const string &player_name) : gravity_mode_on(
        gravity_mode_on), leaderboard_file_name(leaderboard_file_name), player_name(player_name) {
    initialize_grid(grid_file_name);
    read_blocks(blocks_file_name);
    leaderboard.read_from_file(leaderboard_file_name); 
}


void BlockFall::read_blocks(const string &input_file) {
    // TODO: Read the blocks from the input file and initialize "initial_block" and "active_rotation" member variables
    // TODO: For every block, generate its rotations and properly implement the multilevel linked list structure
    //       that represents the game blocks, as explained in the PA instructions.
    // TODO: Initialize the "power_up" member variable as the last block from the input file (do not add it to the linked list!)

    std::ifstream myFile;
    myFile.open(input_file, ios::in);

    std::string input;
    std::string line;
    int emptyLinesCounter = 0;

    if(myFile.is_open()){
        while (std::getline(myFile, line))
        {
            input += line;
            if(line.size() == 0){
                emptyLinesCounter++;
            }
        }
        myFile.close();
    }

    int emptyLinesCounterToCheck = 0;
    myFile.open(input_file, ios::in);
    std::vector<bool> vectorToPush;
    bool powerup = false;

    if(myFile.is_open()){
        while (std::getline(myFile, line))
        {

            if(line.size() == 0){
                emptyLinesCounterToCheck++;
            }else{
                if(emptyLinesCounterToCheck != emptyLinesCounter){
                    if(initial_block == nullptr){
                        initial_block = new Block();
                        ImplementVector(line, initial_block->shape);
                        while (std::getline(myFile, line) && line.size() != 0)
                        {
                            ImplementVector(line, initial_block->shape);
                        }


                        SetRotations(initial_block);

                        active_rotation = initial_block;
                        /*
                        //Setting rotations
                        for(int i = 0; i < 4; i++){
                            Block* rightRotation = new Block();
                            Block* temp;
                            if(initial_block->right_rotation == nullptr){
                                initial_block->right_rotation = rightRotation;
                                rightRotation->left_rotation = initial_block;
                            }else{
                                temp = initial_block; //Block* temp = initial_block;
                                while (temp->right_rotation != nullptr) 
                                {                                    
                                    temp = temp->right_rotation;
                                    
                                }
                                temp->right_rotation = rightRotation;
                                rightRotation->left_rotation = temp;
                            }
                            
                            RotateVector(rightRotation, temp->shape);
                            while (std::getline(myFile, line) && line.size() != 0)
                            {
                                RotateVector(rightRotation, temp->shape);
                            }
                        }*/
                        
                    }else{
                        Block* temp = initial_block;
                        Block* previous = new Block();
                        while (temp != nullptr) 
                        {                   
                            previous = temp;
                            temp = temp->next_block;
                            
                        }
                        temp = new Block();
                        previous->next_block = temp;
                        initialize_next_rotations(previous);
                        ImplementVector(line, temp->shape);
                        while (std::getline(myFile, line) && line.size() != 0)
                        {
                            ImplementVector(line, temp->shape);
                        }


                        SetRotations(temp);
                    }

                    
                    
                }

            }

            //initialize powerup
            if(emptyLinesCounterToCheck == emptyLinesCounter){
                //powerup = true;
                while (std::getline(myFile, line)){ // check if true
                    ImplementVector(line, power_up);
                }
            }

            
        }
    myFile.close();
    }

}

void BlockFall::initialize_grid(const string &input_file) {
    // TODO: Initialize "rows" and "cols" member variables
    // TODO: Initialize "grid" member variable using the command-line argument 1 in main
    
    std::ifstream myFile;
    myFile.open(input_file, ios::in);

    std::string line;

    std::vector<int> vectorToPush;

    int rowCounter = 0;
    if(myFile.is_open()){
        while (std::getline(myFile, line))
        {
            rowCounter++;
            cols = (line.size() / 2) + 1;            

        }
        rows = rowCounter;
        myFile.close();
    }

    for(int i = 0; i < rows; i++){
        for(int j = 0; j < cols; j++){
            vectorToPush.push_back(0);
        }
        grid.push_back(vectorToPush);
        vectorToPush.clear();
    }

    /*
    myFile.open(input_file, ios::in);
    char character;
    if(myFile.is_open()){
        while (myFile.get(character))
        {
            if(character != ' ' && character != '\n'){
                vectorToPush.push_back({0});
            }

            if(character == '\n'){
                grid.push_back(vectorToPush);
            }

        }
        myFile.close();
    }*/

}


BlockFall::~BlockFall() {
    // TODO: Free dynamically allocated memory used for storing game blocks 

    
    Block* temp = initial_block;
    Block* next = nullptr;
    while (temp != nullptr)
    {
        next = temp->next_block;
        delete temp;
        temp = next;

    }

    

    
}



void BlockFall::ImplementVector(std::string line, vector<vector<bool>> &vector){
    std::vector<bool> vectorToPush;

    for(int i = 0; i < line.size(); i++){
        if(line[i] == '1'){
            vectorToPush.push_back(true);
        }else if (line[i] == '0')
        {
            vectorToPush.push_back(false);
        }
        
    }
    vector.push_back(vectorToPush);
    vectorToPush.clear();
}

void BlockFall::RotateVector(Block* vectorToRotate, vector<vector<bool>> &vector){
    std::vector<bool> vectorToPush;
    for(int i = 0; i < vector[0].size(); i++){
        for(int j = 0; j < vector.size(); j++){
            vectorToPush.push_back(vector[vector.size() - j -1][i]);//check
        }
        vectorToRotate->shape.push_back(vectorToPush); //vectorToRotate->shape.push_back(vectorToPush);
        vectorToPush.clear();
    }
    
} 


void BlockFall::SetRotations(Block* block) { // check if you need to put &
    //std::ifstream myFile;
    //myFile.open(input_file, ios::in);
    std::string line;

    //Setting rotations
    for (int i = 0; i < 4; i++) {
        Block* rightRotation = new Block();
        Block* temp = block;
        if (temp->right_rotation == nullptr) {
            RotateVector(rightRotation, temp->shape);
            if (*rightRotation == *block) {
                delete rightRotation;
                temp->right_rotation = temp;
                temp->left_rotation = temp;
                break;
            }
            else {
                temp->right_rotation = rightRotation;
                rightRotation->left_rotation = temp;
            }

        }
        else {
            temp = block; //Block* temp = initial_block;
            Block* previous = nullptr;
            while (temp->right_rotation != nullptr)
            {
                previous = temp;
                temp = temp->right_rotation;

            }
            RotateVector(rightRotation, temp->shape);
            if (i == 3) { //

                block->left_rotation = temp;
                temp->right_rotation = block;
                break;
            }


            if (*rightRotation == *block) { //
                delete temp;

                block->left_rotation = previous;
                previous->right_rotation = block;
                break;
            }

            else {
                temp->right_rotation = rightRotation;
                rightRotation->left_rotation = temp;
            }

        }

        //rightRotation->next_block = temp->next_block;

        // bottom part may be uncessary
        //while (std::getline(myFile, line) && line.size() != 0)
        //{
        //    RotateVector(rightRotation, temp->shape);
        //}
    }
}

void BlockFall::initialize_next_rotations(Block* block) {
    Block* temp = block;
    for (int i = 0; i < 3; i++) {
        
        
         //Block* temp = initial_block;
        temp = temp->right_rotation;
        temp->next_block = block->next_block;
        
        //rightRotation->next_block = temp->next_block;
    }
}