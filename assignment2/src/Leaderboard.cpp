#include "Leaderboard.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>

using namespace std;

void Leaderboard::insert_new_entry(LeaderboardEntry* new_entry) {
    // Find the size of the leaderboard
    int size = 0;
    LeaderboardEntry* current_size = head_leaderboard_entry;
    while (current_size) {
        size++;
        current_size = current_size->next_leaderboard_entry;
    }
    
    if (!head_leaderboard_entry || new_entry->score > head_leaderboard_entry->score) {
        // if it is in head
        new_entry->next_leaderboard_entry = head_leaderboard_entry;
        head_leaderboard_entry = new_entry;
        size++;
    } else {
        LeaderboardEntry *current = head_leaderboard_entry;

        while (current->next_leaderboard_entry && new_entry->score <= current->next_leaderboard_entry->score) {
            current = current->next_leaderboard_entry;
        }

        // if it is not in head
        new_entry->next_leaderboard_entry = current->next_leaderboard_entry;
        current->next_leaderboard_entry = new_entry;
        size++;
    }

    // delete the last element if the size is 11
    if (size > MAX_LEADERBOARD_SIZE) {
        LeaderboardEntry* temp_entry = head_leaderboard_entry;
        LeaderboardEntry* prev_entry = temp_entry;
        
        while (temp_entry->next_leaderboard_entry) {
            prev_entry = temp_entry;
            temp_entry = temp_entry->next_leaderboard_entry;
        }
        delete temp_entry;
        prev_entry->next_leaderboard_entry = nullptr;
    }

}


void Leaderboard::write_to_file(const string& filename) {
    // TODO: Write the latest leaderboard status to the given file in the format specified in the PA instructions
    
    ofstream output_leader(filename);
    if(!output_leader.is_open()){
        cerr << "Cannot open file!" << endl;
        exit(EXIT_FAILURE);
    }
    
    LeaderboardEntry *temp_entry = head_leaderboard_entry;

    while (temp_entry) {
        output_leader << temp_entry->score << " " << temp_entry->last_played << " " << temp_entry->player_name << "\n";
        temp_entry = temp_entry->next_leaderboard_entry;
    }

    output_leader.close();
    
}


void Leaderboard::read_from_file(const string& filename) {
    ifstream input_leaders(filename);

    if (!input_leaders.is_open()) {
        cerr << "Cannot open file: " << filename << endl;
        exit(EXIT_FAILURE);
    }

    string line;

    // Check if the file is empty before attempting to read
    if (getline(input_leaders, line)) {
        istringstream iss(line);
        unsigned long score;
        time_t last_played;
        string player_name;

        iss >> score >> last_played >> player_name;

        LeaderboardEntry* new_entry = new LeaderboardEntry(score, last_played, player_name);
        head_leaderboard_entry = new_entry;
        LeaderboardEntry* temp_entry = head_leaderboard_entry;

        while (getline(input_leaders, line)) {
            istringstream iss(line);
            iss >> score >> last_played >> player_name;

            LeaderboardEntry* new_entry = new LeaderboardEntry(score, last_played, player_name);
            temp_entry->next_leaderboard_entry = new_entry;
            temp_entry = temp_entry->next_leaderboard_entry;
        }
    }

    input_leaders.close();
}



void Leaderboard::print_leaderboard() {
    // TODO: Print the current leaderboard status to the standard output in the format specified in the PA instructions
    cout << "\nLeaderboard\n";
    cout << "-----------\n";

    LeaderboardEntry *temp_entry = head_leaderboard_entry;
    int order = 1;

    while (temp_entry) {
        struct tm *timeinfo = localtime(&temp_entry->last_played);
        char buffer[80];
        strftime(buffer, 80, "%H:%M:%S/%d.%m.%Y", timeinfo);

        cout << order << ". " << temp_entry->player_name << " " << temp_entry->score << " " << buffer << "\n";

        temp_entry = temp_entry->next_leaderboard_entry;
        order++;
    }
}

Leaderboard::~Leaderboard() {
    // TODO: Free dynamically allocated memory used for storing leaderboard entries
    LeaderboardEntry *current = head_leaderboard_entry;

    while (current != nullptr) {
        LeaderboardEntry *temp = current;
        delete temp;
        current = current->next_leaderboard_entry;        
    }
    
    head_leaderboard_entry = nullptr;
}
