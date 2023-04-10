#include <iostream>
#include <fstream>
#include <cmath>
#include "json.hpp"
#include "priorityqueue.h"

using namespace std;

int choose(int n, int k);

int main(int argc, char** argv) {
    if(argc != 2){   // check command line for 2 arguements
            cerr << "Usage: ./createteams [file].json\n";  //print error if not two args
            return 1;   //ends program
        }

    ifstream inFile;    //creates file stream obj
    inFile.open(string(argv[1]));   // opens json file
    nlohmann::json jsonObject;  //creates jsob obj
    if (inFile.is_open()) {     // check if file opens
        inFile >> jsonObject;   //read in file to json obj
    }
    else{
        cerr << "Inpute File Not Open\n";   //if file does not open print error
        return 1; // end program
    }
    inFile.close(); // close input file

    size_t maxSize = choose(jsonObject["metadata"]["numPlayers"], 2); // set maxSize to numplayers choose 2

    PriorityQueue heap(maxSize);   // create priority queue of maxSize size

    double fairRate;    // double var
    int p1, p2; // player 1 and player 2 ints

    for(size_t i = 0; i < maxSize; i++){    // loop for amount of teams
        fairRate = abs(double(jsonObject["teamStats"][i]["winPercentage"])-50); // set fairRate to absolute value of winpercentage-50
        p1 = jsonObject["teamStats"][i]["playerOne"];   // p1 is set to player 1
        p2 = jsonObject["teamStats"][i]["playerTwo"];   // p2 is set to player 2
        heap.insert(make_pair(fairRate, make_pair(p1, p2)));    // insert fairRate as key and p1 and p2 as values
        
    }

    nlohmann::json jsonOutput;  // create json Obj
    map <int, bool> isOnTeam;   // map to check if a player is already on a team
    pair<int, int> player;  // pair of ints for the two players on each team
    int teamNum = 0;    // amount of teams created
    for(size_t i = 1; i < maxSize+1; i++){  // loop for every index in heap
        player = heap.removeMin().second;   // pop heap and set player equal to value pair
        if(isOnTeam[player.first] == false && isOnTeam[player.second] == false){    // if player 1 and 2 both are not in map
            jsonOutput["teams"][teamNum] = {player.first, player.second};   // add team to output json
            teamNum++;  // increment teams
            isOnTeam[player.first] = true;  // set player to true in map
            isOnTeam[player.second] = true; // set player to true in map
        }
        
    }
    
    cout << jsonOutput.dump(2) << endl; // output json obj
}

int choose(int n, int k){   // function to calc n choose k
    if (k == 0) return 1;   // if k is 0 return 1
    return ((n * choose(n - 1, k - 1)) / k);    // return recursive call of n choose k with n-1 and k-1 / 2
}
    
