#include "GameController.h"
#include "iostream"
#include "fstream"
#include <cstdlib>
#include <vector>
#include <ctime>

void GameController::print_grid(vector<vector<int>> init_grid, const vector<vector<bool>> block, const int score, int move_right, int move_left){   
    cout << "Score: " << score << endl;
    cout << "High Score: " << endl;

    int start_col = move_right - move_left;
    for(int i = 0; i < block.size(); i++){
        for(int j = start_col; j < start_col + block[0].size(); j++){
            init_grid[i][j] = block[i][j - start_col];
        }               
    }

    for (const auto& row : init_grid) {
        for (int value : row) {
            cout << (value ? occupiedCellChar : unoccupiedCellChar);
        }
        cout << endl;
    }    
}


void GameController::game_finish(const vector<vector<int>> init_grid, const int score){
    cout << "GAME FINISHED!\n";
    cout << "No more commands .\n";
    
    cout << "Final grid and score:\n" << endl;
    cout << "Score: " << score << endl;
    cout << "High Score: " << endl;

    for (const auto& row : init_grid) {
        for (int value : row) {
            cout << (value ? occupiedCellChar : unoccupiedCellChar);
        }
        cout << endl;
    }    

}


void GameController::game_win(const vector<vector<int>> init_grid, const int score){
    cout << "YOU WIN!\n";
    cout << "No more blocks.\n";
    
    cout << "Final grid and score:\n" << endl;
    cout << "Score: " << score << endl;
    cout << "High Score: " << endl;

    for (const auto& row : init_grid) {
        for (int value : row) {
            cout << (value ? occupiedCellChar : unoccupiedCellChar);
        }
        cout << endl;
    }    

}


void GameController::game_over(const vector<vector<int>> init_grid, const vector<vector<bool>> active_block, const int score){
    cout << "GAME OVER!\n";
    cout << "Next block that couldn't fit:\n";
    for (const auto& row : active_block) {
        for (bool value : row) {
            cout << (value ? occupiedCellChar : unoccupiedCellChar);
        }
        cout << endl;
    }
    cout << endl;
    cout << "Final grid and score:\n" << endl;
    cout << "Score: " << score << endl;
    cout << "High Score: " << endl;

    for (const auto& row : init_grid) {
        for (int value : row) {
            cout << (value ? occupiedCellChar : unoccupiedCellChar);
        }
        cout << endl;
    }    

}


bool GameController::is_top_left_empty(const vector<vector<int>> init_grid, const vector<vector<bool>> active_block){
    int size_row = active_block.size();
    int size_col = active_block[0].size();
    for (int i = 0; i < size_row; i++){
        for(int j = 0; j < size_col; j++){
            if(init_grid[i][j]){
                return false;
            }
        }
    }
    return true;
}


void GameController::move_right(const vector<vector<int>> grid, const vector<vector<bool>> block, int& move_right, int move_left){
    int block_row = block.size();             // row number of block
    int block_col = block[0].size();          // column number of block
    int start_col = move_right - move_left;   // column number where the top left of block is

    // check if it exceeds the grid from end
    if(block_col + start_col == grid[0].size()){
        return;
    }

    // check the right of the block in grid
    for(int i = 0; i < block_row; i++){
        if(grid[i][block_col + start_col] == 1){
            return;
        }
    } 
    
    move_right++;   
}


void GameController::move_left(const vector<vector<int>> grid, const vector<vector<bool>> block, int move_right, int& move_left){
    int block_row = block.size();             // row number of block
    int block_col = block[0].size();          // column number of block
    int start_col = move_right - move_left;   // column number where the top left of block is

    // check if it exceeds the grid from begin
    if(move_left == move_right){
        return;
    }

    // check the left of the block in grid
    for(int i = 0; i < block_row; i++){
        if(grid[i][start_col - 1] == 1){
            return;
        }
    }
    
    move_left++;
}


bool GameController::rotate_right(const vector<vector<int>> grid, const vector<vector<bool>> first_block,
 const vector<vector<bool>> rotated_block, int move_right, int move_left){
    int new_row = rotated_block.size();       // row number of rotated block
    int new_col = rotated_block[0].size();    // column number of rotated block
    int start_col = move_right - move_left;   // column number where the top left of block is(same for initial and rotated)

    // check if it exceeds the grid
    if(start_col + new_col > grid[0].size()){
        return false;
    }

    // check the rotated positions
    for(int i = 0; i < new_row; i++){
        for(int j = start_col; j < new_col + start_col; j++){
            if(grid[i][j] == 1){
                return false;          
            }
        }
    }

    return true;     
}


bool GameController::rotate_left(const vector<vector<int>> grid, const vector<vector<bool>> first_block,
 const vector<vector<bool>> rotated_block, int move_right, int move_left){
    int new_row = rotated_block.size();       // row number of rotated block
    int new_col = rotated_block[0].size();    // column number of rotated block
    int start_col = move_right - move_left;   // column number where the top left of block is(same for initial and rotated)
    
    // check if it exceeds the grid
    if(start_col + new_col > grid[0].size()){
        return false;
    }

    // check the rotated positions
    for(int i = 0; i < new_row; i++){
        for(int j = start_col; j < start_col + new_col; j++){
            if(grid[i][j] == 1){
                return false;          
            }
        }
    }

    return true;
}


void GameController::drop_gravity_off(vector<vector<int>>& grid, const vector<vector<bool>> block, int move_right, int move_left, unsigned long& score){
    int block_row = block.size();             // row number of block
    int block_col = block[0].size();          // column number of block
    int start_col = move_right - move_left;   // column number where the top left of block is
    int drop_num = 0;                         
    int trues = 0;

    for(int col = 0; col < block_col; col++){
        // find the bottom true in column
        int base_true_row = 0;
        for(int row = block_row - 1; row >= 0; row--){
            if(block[row][col]){
                base_true_row = row;
                break;
            }
        }

        // find the temp drop num for each column
        int temp_drop_num = 0;
        for(int new_row = base_true_row + 1; new_row < grid.size(); new_row++){
            if(grid[new_row][col + start_col] == 0){
                temp_drop_num++;
            }
            else{
                break;
            }
        }

        // for first column
        if(drop_num == 0){
            drop_num = temp_drop_num;
        }

        // if the small drop occurs
        if(drop_num > temp_drop_num){
            drop_num = temp_drop_num;
        }
    }

    // drop the row drop_num
    for(int row = 0; row < block_row; row++){
        for(int col = 0; col < block_col; col++){
            if(block[row][col]){
                grid[row + drop_num][col + start_col] = block[row][col];
            }
        }
    }

    // find the parts of the block and evaluate score
    for(int row = 0; row < block_row; row++){
        for(int col = 0; col < block_col; col++){
            if(block[row][col]){
                trues++;
            }
        }
    }
    score += trues * drop_num;
    
}


void GameController::drop_gravity_on(vector<vector<int>>& grid, const vector<vector<bool>> block, int move_right, int move_left, unsigned long& score){
    int block_row = block.size();             // row number of block
    int block_col = block[0].size();          // column number of block
    int start_col = move_right - move_left;   // column number where the top left of block is   

    for(int col = 0; col < block_col; col++){
        // find the bottom true in column
        int base_true_row = 0;
        for(int row = block_row - 1; row >= 0; row--){
            if(block[row][col]){
                base_true_row = row;
                break;
            }
        }

        // find the drop num for col
        int drop_num = 0;
        for(int new_row = base_true_row + 1; new_row < grid.size(); new_row++){
            if(grid[new_row][col + start_col] == 0){
                drop_num++;
            }
            else{
                break;
            }
        }

        // drop the row drop_num and calculate score       
        bool found = false;
        for(int row = block_row - 1; row >= 0; row--){           
            if(block[row][col]){
                grid[row + drop_num][col + start_col] = block[row][col];
                found = true;
                score += drop_num;
            }
            else{
                if(found){
                    drop_num++;
                    
                }               
            }                   
        }      
    }
}


void GameController::clear_full_row(vector<vector<int>>& grid, unsigned long& score){
    int rows = grid.size();
    int cols = grid[0].size();
    bool printed = false;

    for(int i = 0; i < rows; i++){
        bool full_row = true;
        for(int j = 0; j < cols; j++){
            if(grid[i][j] == 0){
                full_row = false;
                break;
            }
        }
        
        if(full_row){
            // print the before cleaning
            if(!printed){
                cout << "before cleaning\n";
                for (const auto& row : grid) {
                    for (int value : row) {
                        cout << (value ? occupiedCellChar : unoccupiedCellChar);
                    }
                    cout << endl;
                }    
                printed = true;
            }

            // clear the full row
            for (int j = 0; j < cols; ++j) {
                grid[i][j] = 0;
            }

            // Drop 1s above the cleared row
            for (int k = i; k > 0; --k) {
                for (int j = 0; j < cols; ++j) {
                    grid[k][j] = grid[k - 1][j];
                }
            }

            // Clear the top row
            for (int j = 0; j < cols; ++j) {
                grid[0][j] = 0;
            }

            score += cols;
        }
        
    }
}


void GameController::switch_on(vector<vector<int>>& grid){
    int rows = grid.size();
    int cols = grid[0].size();

    for(int col = 0; col < cols; col++){
        int number_of_1s = 0;
        for(int row = 0; row < rows; row++){
            if(grid[row][col] == 1){
                number_of_1s++;
            }
        }

        for(int row_1 = rows - 1; row_1 >= rows - number_of_1s; row_1--){
            grid[row_1][col] = 1;
        }
        
        for(int row_0 = rows - number_of_1s - 1; row_0 >= 0; row_0--){
            grid[row_0][col] = 0;
        }
    }
}


void GameController::check_power_up(vector<vector<int>>& grid, const vector<vector<bool>> power_up, unsigned long& score){
    int rows = grid.size();
    int cols = grid[0].size();
    int p_rows = power_up.size();
    int p_cols = power_up[0].size();

    for (int i = 0; i <= rows - p_rows; ++i) {
        for (int j = 0; j <= cols - p_cols; ++j) {
            bool found_power_up = true;

            // Check if power-up is in the grid
            for (int p_row = 0; p_row < p_rows; p_row++) {
                for (int p_col = 0; p_col < p_cols; p_col++) {
                    if (power_up[p_row][p_col] != grid[i + p_row][j + p_col]) {
                        found_power_up = false;
                        break;
                    }
                }
                if (!found_power_up) {
                    break;
                }
            }
          
            // If power-up is found, clear
            if (found_power_up) {
                score += 1000;
                for (int g_row = 0; g_row < rows; g_row++) {
                    for (int g_col = 0; g_col < cols; g_col++) {
                        if(grid[g_row][g_col] == 1){
                            score ++;
                            grid[g_row][g_col] = 0;
                        }
                    }
                }
            }
        }
    }  
}


bool GameController::play(BlockFall& game, const string& commands_file){

    // TODO: Implement the gameplay here while reading the commands from the input file given as the 3rd command-line
    //       argument. The return value represents if the gameplay was successful or not: false if game over,
    //       true otherwise.

    
    if(!is_top_left_empty(game.grid, game.active_rotation->shape)){
        return false;
    }

    ifstream commands(commands_file);
    if(!commands.is_open()){
        std::cerr << "Cannot open file!" << std::endl;
        exit(EXIT_FAILURE);
    }

    string command;
    while (commands >> command) {
        if (command == "PRINT_GRID") {
            print_grid(game.grid, game.active_rotation->shape, game.current_score, move_right_num, move_left_num);
        }

        else if (command == "ROTATE_RIGHT") { 
                      
            bool check_right = rotate_right(game.grid, game.active_rotation->shape, game.active_rotation->right_rotation->shape, move_right_num, move_left_num);
            if(check_right){
                game.active_rotation = game.active_rotation->right_rotation;
            }
                       
        } 

        else if (command == "ROTATE_LEFT") { 
                    
            bool check_left = rotate_left(game.grid, game.active_rotation->shape, game.active_rotation->left_rotation->shape, move_right_num, move_left_num);
            if(check_left){
                game.active_rotation = game.active_rotation->left_rotation;
            } 
                   
        }

        else if (command == "MOVE_RIGHT") {
            move_right(game.grid, game.active_rotation->shape, move_right_num, move_left_num);
        } 

        else if (command == "MOVE_LEFT") {
            move_left(game.grid, game.active_rotation->shape, move_right_num, move_left_num);
        }

        else if (command == "DROP") {
            // drop the block
            if(game.gravity_mode_on){
                drop_gravity_on(game.grid, game.active_rotation->shape, move_right_num, move_left_num, game.current_score);
                check_power_up(game.grid, game.power_up, game.current_score);
                clear_full_row(game.grid, game.current_score);
            }
            else{
                drop_gravity_off(game.grid, game.active_rotation->shape, move_right_num, move_left_num, game.current_score);
                check_power_up(game.grid, game.power_up, game.current_score);
                clear_full_row(game.grid, game.current_score);
            }

            if(game.active_rotation->next_block == nullptr){
                // you win there is no more block
                game_win(game.grid, game.current_score);
                game_finish(game.grid, game.current_score);
                time_t current_time = time(nullptr);
                LeaderboardEntry* player_new =  new LeaderboardEntry(game.current_score, current_time, game.player_name);
                game.leaderboard.insert_new_entry(player_new);
                game.leaderboard.print_leaderboard();
                game.leaderboard.write_to_file(game.leaderboard_file_name);
                return true;
            }
            else{
                game.active_rotation = game.active_rotation->next_block;
                if(!is_top_left_empty(game.grid, game.active_rotation->shape)){
                    //game over, new block cannot entry into grid
                    game_finish(game.grid, game.current_score);
                    time_t current_time = time(nullptr);
                    LeaderboardEntry* player_new =  new LeaderboardEntry(game.current_score, current_time, game.player_name);
                    game.leaderboard.insert_new_entry(player_new);
                    game.leaderboard.print_leaderboard();
                    game.leaderboard.write_to_file(game.leaderboard_file_name);
                    return false;
                }  
                else{                   
                    move_right_num = 0;
                    move_left_num = 0;
                }              
            }
        }

        else if (command == "GRAVITY_SWITCH") {
            if(game.gravity_mode_on){
                game.gravity_mode_on = false;
            }
            else{
                game.gravity_mode_on = true;
                switch_on(game.grid);
                check_power_up(game.grid, game.power_up, game.current_score);
                clear_full_row(game.grid, game.current_score);
            }
        }

        else {
            cout << "Unknown command: " << command << endl;
        }
    }

    commands.close();

    // game finish, there is no more commnand
    game_finish(game.grid, game.current_score);
    time_t current_time = time(nullptr);
    LeaderboardEntry* player_new =  new LeaderboardEntry(game.current_score, current_time, game.player_name);
    game.leaderboard.insert_new_entry(player_new);
    game.leaderboard.print_leaderboard();
    game.leaderboard.write_to_file(game.leaderboard_file_name);
    return true;
}
