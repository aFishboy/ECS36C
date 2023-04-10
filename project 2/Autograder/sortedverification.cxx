#include "json.hpp"
#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, char** argv) {

    if(argc != 2){   // check command line for 2 arguements
            cerr << "Usage: ./sortedverification [file_name].json\n";  //print error if not two args
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

    nlohmann::json consecObj;   // create new json object to print to stdout
    int inversions = 0; // variable to check how many samples with inversions
    for (auto itr = jsonObject.begin(); itr.key() != "metadata"; ++itr) {   // iterate through json obj until reaching metadata
        int arrIndex = 0;   //set array index to 0
        long temp = -2147483648;    // this is the lowest value can be in the array so no comparison will be wrong before it is initialized
        int wasInverInSamp = 0; //used to check if we had an inversion in the sample
        //std::cout << "key: " << itr.key() << " value: " << itr.value() << endl;
        for (auto arrayItr = jsonObject[itr.key()].begin(); arrayItr != jsonObject[itr.key()].end(); ++arrayItr) {  //iterate through sample array until end
            if (temp > *arrayItr){  // if the previous element is greater than current we have an inversion enter if
                wasInverInSamp++;   // increment was Inversion counter
                // the new json obj gets consecutive inv added  at the index and values
                consecObj[itr.key()]["ConsecutiveInversions"][to_string(arrIndex-1)] = {temp, *arrayItr};   
            }
            if (wasInverInSamp == 1){   // if we have had one inversion in a sample enter only enters once per sample
                inversions++;   // increment for adding samples with inversions later
                consecObj[itr.key()]["sample"] = itr.value();   //add the sample to consecObj json obj
                wasInverInSamp++;   // increment was inversion
            }
            arrIndex++; // increment array index
            temp = *arrayItr;   // set temp to current what the current iterator is pointing to
        }


    }
    consecObj["metadata"]["arraySize"] = jsonObject["metadata"]["arraySize"]; // adds metadata array size to ouput json
    consecObj["metadata"]["file"] = argv[1];    // adds metadat file to ouput json
    consecObj["metadata"]["numSamples"] = jsonObject["metadata"]["numSamples"]; // adds metadata number of samples to ouput json
    consecObj["metadata"]["samplesWithInversions"] = inversions;    // adds metadata samples with inversions to ouput json

    cout << consecObj.dump(2) << "\n";  // outputs json obj to console
    cout << endl;   //formatting

    return 0;   // end program
}