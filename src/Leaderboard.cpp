#include "Leaderboard.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

void Leaderboard::insert_new_entry(LeaderboardEntry * new_entry) {
    // TODO: Insert a new LeaderboardEntry instance into the leaderboard, such that the order of the high-scores
    //       is maintained, and the leaderboard size does not exceed 10 entries at any given time (only the
    //       top 10 all-time high-scores should be kept in descending order by the score).


    if(head_leaderboard_entry == nullptr){
        head_leaderboard_entry = new_entry;

    }else{

        LeaderboardEntry* temp = head_leaderboard_entry;
        LeaderboardEntry* previous_entry = nullptr;

        

        while (temp != nullptr)
        {
            
            if(new_entry->score > temp->score){ // check
                
                //new_entry->next_leaderboard_entry = head_leaderboard_entry;
                //head_leaderboard_entry = new_entry;
                if (previous_entry == nullptr) {
                    new_entry->next_leaderboard_entry = head_leaderboard_entry;
                    head_leaderboard_entry = new_entry;

                    break;
                }
                
                previous_entry->next_leaderboard_entry = new_entry;
                new_entry->next_leaderboard_entry = temp;
                entry_number++;
                break;

            }else{/*
                if (previous_entry == nullptr) {
                    return;
                }
                if (previous_entry->next_leaderboard_entry == temp) {
                    break;
                }
                previous_entry->next_leaderboard_entry = new_entry;
                new_entry->next_leaderboard_entry = temp;*/
                if (temp->next_leaderboard_entry == nullptr && entry_number != 10) {
                    if (new_entry->player_name == temp->player_name) {
                        break;
                    }
                    temp->next_leaderboard_entry = new_entry;
                }
            }

            previous_entry = temp;
            temp = temp->next_leaderboard_entry;
        }
        

    }

    //previous->next_leaderboard_entry = new_entry;

}

void Leaderboard::write_to_file(const string& filename) {
    // TODO: Write the latest leaderboard status to the given file in the format specified in the PA instructions

    std::ofstream myFile(filename);
    
    if(myFile.is_open()){
        int number_of_players = 0;
        LeaderboardEntry* temp = head_leaderboard_entry;
        while (number_of_players < 10 && temp != nullptr)
        {
            number_of_players++;
            myFile << temp->score << " " << temp->last_played << " " << temp->player_name << "\n";
            temp = temp->next_leaderboard_entry;
        }
        
        myFile.close();
    }


    

}

void Leaderboard::read_from_file(const string& filename) {
    // TODO: Read the stored leaderboard status from the given file such that the "head_leaderboard_entry" member
    //       variable will point to the highest all-times score, and all other scores will be reachable from it
    //       via the "next_leaderboard_entry" member variable pointer.

    std::ifstream myFile;
    myFile.open(filename, ios::in);

    std::string line;
    std:string variable;
    vector<string> variables;

    if(myFile.is_open()){
        unsigned long score;
        time_t last_played;
        std::string player_name;
        LeaderboardEntry* entry = nullptr;

        while (std::getline(myFile, line)){
            std::stringstream ss(line);
            while(std::getline(ss, variable, ' ')){
                variables.push_back(variable);
            }

            

            score = std::stoul(variables[0]);
            
            last_played = time_t(static_cast<time_t>(std::stoi(variables[1]))); //dont forget to fix this
            player_name = variables[2];

            if(entry == nullptr){
                entry = new LeaderboardEntry(score, last_played, player_name);
                head_leaderboard_entry = entry;
            }else{
                entry->next_leaderboard_entry = new LeaderboardEntry(score, last_played, player_name);
                entry = entry->next_leaderboard_entry;
            }

        }

        
        myFile.close();
    }

    

}


void Leaderboard::print_leaderboard() {
    // TODO: Print the current leaderboard status to the standard output in the format specified in the PA instructions
    int number_of_player = 0;
    LeaderboardEntry* temp = head_leaderboard_entry;
    while (number_of_player < 10 && temp !=nullptr)
    {
        
        std::cout << temp->score << " " << temp->last_played << " " << temp->player_name << "\n";
        temp = temp->next_leaderboard_entry;
        number_of_player++;
    }
    
}

Leaderboard::~Leaderboard() {
    // TODO: Free dynamically allocated memory used for storing leaderboard entries
    LeaderboardEntry* temp  = head_leaderboard_entry;
    LeaderboardEntry* next_leaderboard  = nullptr; // check
    while (temp != nullptr)
    {
        next_leaderboard = temp->next_leaderboard_entry;
        delete temp;
        temp = next_leaderboard;
    }
    
}
