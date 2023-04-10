#include <iostream>
#include <fstream>
#include "priorityqueue.h"

using namespace std;

int main(int argc, char** argv) {

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

    size_t maxSize = jsonObject["metadata"]["maxHeapSize"]; // get maxSize variable
    PriorityQueue heap(maxSize);    // create priority queue of size maxSize

    // iterate through each key until metadata
    for (auto itr = jsonObject.begin(); itr.key() != "metadata"; ++itr) { 
        if(jsonObject[itr.key()]["operation"] == "insert"){ // if operation is insert
            size_t keyToInsert = jsonObject[itr.key()]["key"];  //set key to jsonObj key
            heap.insert(keyToInsert);}   // insert key
        else{
            heap.removeMin();}}   // else remove the root
        

    nlohmann::json outputJson;  //creates json obj for output
    int operations = jsonObject["metadata"]["numOperations"];   // gets amount of operations
    outputJson = heap.JSON(operations); // create output json from priority queue method that was given to us

    cout << outputJson.dump(2) << "\n";  // outputs json output to console
    

}
