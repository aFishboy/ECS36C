#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include "insertionsort.h"
#include "mergesort.h"
#include "quicksort.h"
#include "json.hpp"

using namespace std;

void printData(ofstream& outFile, double time, int comp, int mem);

int main(int argc, char** argv) {
     if(argc != 2){   // check command line for 2 arguements
            cerr << "Usage: ./timealgorithms [file_name].json\n";  //print error if not two args
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

    ofstream outFile;   // create ofstream object
    outFile.open("output.csv"); //open output file
    if (!outFile.is_open()) {   // check to see if output file opened
        cerr << "Output File Not Open"; // if not open err
        return 1;   // end program if file does not open
    }

    cout <<"Sample,InsertionSortTime,InsertionSortCompares,"    // header line for console
            <<"InsertionSortMemaccess,MergeSortTime,MergeSortCompares," // header line for console
            <<"MergeSortMemaccess,QuickSortTime,QuickSortCompares," // header line for console
            <<"QuickSortMemaccess\n";   // header line for console
    outFile <<"Sample,InsertionSortTime,InsertionSortCompares," // header line for csv file
            <<"InsertionSortMemaccess,MergeSortTime,MergeSortCompares," // header line for csv file
            <<"MergeSortMemaccess,QuickSortTime,QuickSortCompares," // header line for csv file
            <<"QuickSortMemaccess\n";   // header line for csv file

    vector <int> sampVec;   // create vector to store sample array data
    chrono::high_resolution_clock::time_point start;    // create clock start object
    int comparisons = 0;    // var to calc comparisons of each alg
    int memAccess = 0;  // var to calc memory accesses of each alg

chrono::high_resolution_clock::time_point start;
start = chrono::high_resolution_clock::now();
auto end = chrono::high_resolution_clock::now();
double elapsed_us = chrono::duration<double, std::micro>(end - start).count();

    for (auto itr = jsonObject.begin(); itr.key() != "metadata"; ++itr) {   // loop for each sample until hitting "metadata"
        cout << itr.key();  //output the sample number to console
        outFile << itr.key();   //output the sample number to csv

        sampVec = itr.value().get<std::vector<int>>();  // make vector = to the current sample
        start = chrono::high_resolution_clock::now();   // start the timer
        InsertionSort(&sampVec, comparisons, memAccess);    // run insertion with the vector comparisons and memory accesses as args
        auto end = chrono::high_resolution_clock::now();    // start end clock
        double elapsed_us = chrono::duration<double, std::micro>(end - start).count();   // calculates time used
        printData(outFile, elapsed_us, comparisons, memAccess); // prints data to console and csv
        comparisons = 0;    // reset comparisons to be used again
        memAccess = 0;  // reset memory accesses to be used again


        sampVec = itr.value().get<std::vector<int>>();  // make vector = to the current sample
        start = chrono::high_resolution_clock::now();   // start the timer
        MergeSort(&sampVec, comparisons, memAccess);    // run mergesort with the vector comparisons and memory accesses as args
        end = chrono::high_resolution_clock::now(); // start end clock
        elapsed_us = chrono::duration<double, std::micro>(end - start).count();   // calculates time used
        printData(outFile, elapsed_us, comparisons, memAccess); // prints data to console and csv
        comparisons = 0;    // reset comparisons to be used again
        memAccess = 0;  // reset memory accesses to be used again

        sampVec = itr.value().get<std::vector<int>>();  // make vector = to the current sample
        start = chrono::high_resolution_clock::now();   // start the timer
        QuickSort(&sampVec, comparisons, memAccess);    // run quicksort with the vector comparisons and memory accesses as args
        end = chrono::high_resolution_clock::now(); // start end clock
        elapsed_us = chrono::duration<double, std::micro>(end - start).count();   // calculates time used
        printData(outFile, elapsed_us, comparisons, memAccess); // prints data to console and csv
        comparisons = 0;    // reset comparisons to be used again
        memAccess = 0;  // reset memory accesses to be used again

        cout << endl;   // formatting for console
        outFile << endl;    //formatting for csv
        
    }
    outFile.close();    // close output file

    return 0;   //end program
}

void printData(ofstream& outFile, double time, int comp, int mem){
    cout << "," << fixed << setprecision(6) << time << "," << comp << "," << mem;   // print all the data needed for console
    outFile << "," << fixed << setprecision(6) << time << "," << comp << "," << mem;    // print all the data needed for csv
}