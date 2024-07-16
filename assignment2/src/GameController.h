#ifndef PA2_GAMECONTROLLER_H
#define PA2_GAMECONTROLLER_H

#include "BlockFall.h"

using namespace std;

class GameController {
public:
    int move_right_num = 0; // after move right, it is for increment the index
    int move_left_num = 0; // after move left, it is for increment the index
    bool play(BlockFall &game, const string &commands_file); // Function that implements the gameplay
    
    void game_over(const vector<vector<int>> init_grid, const vector<vector<bool>> active_block, const int score); // game over if block can not entry into grid
    void game_win(const vector<vector<int>> init_grid, const int score); // game win if there is no block
    void game_finish(const vector<vector<int>> init_grid, const int score); // game finish if there is no command 
    void print_grid(vector<vector<int>> init_grid, const vector<vector<bool>> block, const int score, int move_right, int move_left); // print the current grid
 
    bool is_top_left_empty(const vector<vector<int>> init_grid, const vector<vector<bool>> active_block); // to check the top left of grid before an incoming block 

    void move_right(const vector<vector<int>> grid, const vector<vector<bool>> block, int& move_right, int move_left); // move the block one index to right
    void move_left(const vector<vector<int>> grid, const vector<vector<bool>> block, int move_right, int& move_left); // move the block one to index left

    bool rotate_right(const vector<vector<int>> grid, const vector<vector<bool>> first_block, const vector<vector<bool>> rotated_block,
     int move_right, int move_left); // rotate the block to right
    bool rotate_left(const vector<vector<int>> grid, const vector<vector<bool>> first_block, const vector<vector<bool>> rotated_block,
     int move_right, int move_left); // rotate the block to left

    void drop_gravity_off(vector<vector<int>>& grid, const vector<vector<bool>> block, int move_right, int move_left, unsigned long& score);
    void drop_gravity_on(vector<vector<int>>& grid, const vector<vector<bool>> block, int move_right, int move_left, unsigned long& score);
    void clear_full_row(vector<vector<int>>& grid, unsigned long& score);
    void check_power_up(vector<vector<int>>& grid, const vector<vector<bool>> power_up, unsigned long& score);
    void switch_on(vector<vector<int>>& grid);
};

#endif //PA2_GAMECONTROLLER_H
