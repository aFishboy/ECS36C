#include <iostream>
#include <fstream>
#include "AVL.h"

using namespace std;

int main(int argc, char* argv[]){
    if(argc != 2){   // check command line for 2 arguements
            cerr << "Usage: ./buildheap [file].json\n";  //print error if not two args
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

    AVL avl1;

    for (auto itr = jsonObject.begin(); itr.key() != "metadata"; ++itr){
        //assert operation is insert
        avl1.Insert(jsonObject[itr.key()]["key"]);
    }
   
 

    cout << avl1.JSON();

    // int test;
    // test = (5 < 10);
    // cout << test << endl;
    

    return 0;
}