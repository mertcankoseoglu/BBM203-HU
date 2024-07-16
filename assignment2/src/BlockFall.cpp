#include "BlockFall.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstdlib>


BlockFall::BlockFall(string grid_file_name, string blocks_file_name, bool gravity_mode_on, const string &leaderboard_file_name, const string &player_name) : gravity_mode_on(
        gravity_mode_on), leaderboard_file_name(leaderboard_file_name), player_name(player_name) {
        initialize_grid(grid_file_name);
        read_blocks(blocks_file_name);
        leaderboard.read_from_file(leaderboard_file_name);
}

// it takes shape of block as parameter and return its left rotated version
vector<vector<bool>> BlockFall::rotate_left(const vector<vector<bool>>& shape_of_block){
    int rows = shape_of_block.size();
    int cols = shape_of_block[0].size();

    vector<vector<bool>> rotated(cols, vector<bool>(rows, false));

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            rotated[cols - 1 - j][i] = shape_of_block[i][j];
        }
    }
    return rotated;
}

// it takes shape of block as parameter and return its right rotated version
vector<vector<bool>> BlockFall::rotate_right(const vector<vector<bool>>& shape_of_block){
    int rows = shape_of_block.size();
    int cols = shape_of_block[0].size();

    vector<vector<bool>> rotated(cols, vector<bool>(rows, false));

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            rotated[j][rows - 1 - i] = shape_of_block[i][j];
        }
    }
    return rotated;
}

void BlockFall::read_blocks(const string &input_file) {  
    // TODO: Read the blocks from the input file and initialize "initial_block" and "active_rotation" member variables
    // TODO: For every block, generate its rotations and properly implement the multilevel linked list structure
    //       that represents the game blocks, as explained in the PA instructions.
    // TODO: Initialize the "power_up" member variable as the last block from the input file (do not add it to the linked list!)
    
    // read the input file and add blocks to the temporary 3d vector
    vector<vector<vector<bool>>> temp_vec;

    ifstream block_file(input_file);
    if(!block_file.is_open()){
        std::cerr << "Cannot open file!" << std::endl;
        exit(EXIT_FAILURE);
    }

    string line;
    while (getline(block_file, line)) {    
        if (line[0] == '[') {
            // Start of a new block
            vector<vector<bool>> block;
            vector<bool> row;
                for (char c : line) {
                    if (c == '0') {
                        row.push_back(false);
                    }else if (c == '1'){
                        row.push_back(true);
                    }
                }
                // Add the row to the current block
                block.emplace_back(row);

            while(getline(block_file, line)){
                if (line.empty()){
                    break;
                }else{
                    vector<bool> row;
                    for (char c : line) {
                        if (c == '0') {
                            row.push_back(false);
                        }else if (c == '1'){
                            row.push_back(true);
                        }
                    }
                    // Add the row to the current block
                    block.emplace_back(row);
                }
                
            }
            temp_vec.emplace_back(block);
        }   
    }
    block_file.close();

    if(temp_vec.empty()){
        return;
    }

    // last block of the temporary 3d vector is power up
    power_up = temp_vec[temp_vec.size()-1];

    // shape of initial block is the first block in temporary 3d vector.
    initial_block = new Block();
    initial_block->shape = temp_vec[0];
    active_rotation = initial_block;
    
    Block* current_block = initial_block;
    for(int i = 1; i < temp_vec.size(); i++){

        // create circular doubly linked list for all blocks
        Block* left_block = new Block();
        left_block->shape = rotate_left(current_block->shape); 
        Block* right_block = new Block();
        right_block->shape = rotate_right(current_block->shape);
       
        // if the circular list has 1 elemnet
        if(*right_block == *current_block && *left_block == *current_block){
            delete left_block;
            delete right_block;

            current_block->left_rotation = current_block;
            current_block->right_rotation = current_block;

            // Link the rest of the blocks
            if(i == temp_vec.size()-1){
                current_block->next_block = nullptr;
                break;
            }
            Block* new_block = new Block();
            new_block->shape = temp_vec[i];
            current_block->next_block = new_block;
            current_block = new_block;
        }

        // if the circular list has 2 elemnets
        else if(*left_block == *right_block){  
            delete left_block;

            current_block->left_rotation = right_block;
            current_block->right_rotation = right_block;
            right_block->left_rotation = current_block;
            right_block->right_rotation = current_block;
            
            // Link the rest of the blocks
            if(i == temp_vec.size()-1){
                current_block->next_block = nullptr;
                break;
            }
            
            Block* new_block = new Block();
            new_block->shape = temp_vec[i];
            current_block->next_block = new_block;
            right_block->next_block = new_block;
            current_block = new_block;
        }

        // if the circular list has 4 elemnets
        else{
            Block* new_right = new Block();
            current_block->left_rotation = left_block;
            current_block->right_rotation = right_block;            
            new_right->shape = rotate_right(right_block->shape);
            right_block->left_rotation = current_block;
            right_block->right_rotation = new_right;
            new_right->left_rotation = right_block;
            new_right->right_rotation = left_block;
            left_block->right_rotation = current_block;
            left_block->left_rotation = new_right;
            
            // Link the rest of the blocks
            if(i == temp_vec.size()-1){
                current_block->next_block = nullptr;
                break;
            }
            Block* new_block = new Block();
            
            new_block->shape = temp_vec[i];
            current_block->next_block = new_block;
            right_block->next_block = new_block;
            left_block->next_block = new_block;
            new_right->next_block = new_block;
            current_block = new_block;
        }
    }
    
}

void BlockFall::initialize_grid(const string &input_file) {
    // TODO: Initialize "rows" and "cols" member variables
    // TODO: Initialize "grid" member variable using the command-line argument 1 in main
    ifstream grid_file(input_file);
    if(!grid_file.is_open()){
        cerr << "Cannot open file!" << endl;
        exit(EXIT_FAILURE);
    }

    string line;
    rows = 0; 

    while (getline(grid_file, line)){
        vector<int> row;
        stringstream str_stream(line);
        int value;
        while (str_stream >> value){
            row.emplace_back(value);
        }

        grid.emplace_back(row);
        cols = row.size();
        rows++;    
    }
    grid_file.close();
}

BlockFall::~BlockFall() {
    // TODO: Free dynamically allocated memory used for storing game blocks
    
    Block* current_block = initial_block;
    while (current_block != nullptr) {
        Block* block_to_delete = current_block;
        current_block = current_block->next_block;
        if(block_to_delete == block_to_delete->right_rotation){
                delete block_to_delete;
        }
        else if(block_to_delete->left_rotation == block_to_delete->right_rotation){  
            delete block_to_delete->left_rotation;
            delete block_to_delete;
            
        }
        else{
            delete block_to_delete->right_rotation->right_rotation;
            delete block_to_delete->right_rotation;
            delete block_to_delete->left_rotation;
            delete block_to_delete;
        }
    }
}

