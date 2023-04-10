#include "json.hpp"
#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, char** argv) {

    if(argc != 3){   // check command line for 3 arguements
        cerr << "Usage: ./consistentresultverification [file_name1].json [file_name2].json\n";  //print error if not three args
        return 1;   //ends program
    }

    ifstream inFile;    //creates file stream obj
    inFile.open(string(argv[1]));   // opens json file
    nlohmann::json jsonObject1;  //creates jsob obj
    if (inFile.is_open()) {     // check if file opens
        inFile >> jsonObject1;   //read in file to json obj
    }
    else{
        cerr << "Inpute File Not Open\n";   //if file does not open print error
        return 1; // end program
    }
    inFile.close(); // close input file

     inFile.open(string(argv[2]));   // opens json file
    nlohmann::json jsonObject2;  //creates jsob obj
    if (inFile.is_open()) {     // check if file opens
        inFile >> jsonObject2;   //read in file to json obj
    }
    else{
        cerr << "Inpute File Not Open\n";   //if file does not open print error
        return 1; // end program
    }
    inFile.close(); // close input file

    nlohmann::json consistentObj;   // create new json object to print to stdout

    auto itr2 = jsonObject2.begin();    // create second itr to go through second json obj
    int conflictions = 0;   // intitialize conflictions to 0
    for (auto itr1 = jsonObject1.begin(); itr1.key() != "metadata"; ++itr1) {   // iterate through json obj until reaching metadata
        int arrIndex = 0;   //set array index to 0
        int wasConflictInSamp = 0; //used to check if we had an inversion in the sample
        auto arrayItr2 = jsonObject2[itr2.key()].begin();
        for (auto arrayItr1 = jsonObject1[itr1.key()].begin(); arrayItr1 != jsonObject1[itr1.key()].end(); ++arrayItr1) {  //iterate through sample array until end
            if (*arrayItr1 != *arrayItr2){  // if the previous element is greater than current we have an inversion enter if
                wasConflictInSamp++;   // increment was Inversion counter
                consistentObj[itr1.key()][argv[2]] = itr2.value();  // add the array value to the proper sample
                consistentObj[itr1.key()]["Mismatches"][to_string(arrIndex)] = {*arrayItr1, *arrayItr2};    // add mismatch to json output
                consistentObj[itr1.key()][argv[1]] = itr1.value();  // add the array value to the proper sample
            }
            if (wasConflictInSamp == 1){   // if we have had one inversion in a sample enter only enters once per sample
                conflictions++;   // increment for adding samples with inversions later
                wasConflictInSamp++;
            }
            arrIndex++; // increment array index
            ++arrayItr2;    // increment the array position of the second file
        }

        ++itr2; // increment the iterator of the second file key values
    }

    consistentObj["metadata"]["File1"]["arraySize"] = jsonObject1["metadata"]["arraySize"]; // adds metadata File1 array size to ouput json
    consistentObj["metadata"]["File1"]["name"] = argv[1];    // adds metadat file name to ouput json
    consistentObj["metadata"]["File1"]["numSamples"] = jsonObject1["metadata"]["numSamples"];    // adds metadata numSamples to ouput json

    consistentObj["metadata"]["File2"]["arraySize"] = jsonObject2["metadata"]["arraySize"]; // adds metadata File2 array size to ouput json
    consistentObj["metadata"]["File2"]["name"] = argv[2];    // adds metadata file name to ouput json
    consistentObj["metadata"]["File2"]["numSamples"] = jsonObject2["metadata"]["numSamples"];    // adds metadata numSamples to ouput json
    consistentObj["metadata"]["samplesWithConflictingResults"] = conflictions;  // adds metadata samplesWithConflictingResults to ouput json


    cout << consistentObj.dump(2) << "\n";  // outputs json obj to console
    return 0;   // end program
}