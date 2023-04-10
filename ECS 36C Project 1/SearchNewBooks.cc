#include <iostream>
#include <fstream>
#include <vector>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <chrono> // NOLINT (build/c++11)

using namespace std;

class BookClass{
    private: 
        int ISBN;
        string language, condition;
    public:
        BookClass(){ //default constructor
            
        }
        
        void assignBook(int ISBN, string language, string condition){ //method to make new book objects while looping through file
            this->ISBN = ISBN;
            this->language = language;
            this->condition = condition;
        }
        void showBook(){    //shows the contents of the current book object
            cout << "ISBN: " << ISBN << endl;
            cout << "language: " << language << endl;
            cout << "condition: " << condition << endl;
        }
        int getISBN(){  
            return ISBN;
        }
        string getLang(){
            return language;
        }
        string getCon(){
            return condition;
        }

        // < operator overloading for sorting book objects
        bool operator<(BookClass& bk){  
            // if the two ISBNS being compared are not equal
            if (ISBN != bk.ISBN)    
                return ISBN < bk.ISBN;
            // if ISBN == ISBN then check condition
            if (condition != bk.condition){ 
                if (condition == "new"){
                    return true;
                }
                if (condition == "used"){
                    if (bk.condition == "new"){
                        return false;
                    }
                    return true;
                } 
                return false;              
            }// if ISBN and condition are = compare language
            return language < bk.language;  }

        bool operator==(BookClass& bk){     // overloading for comparing books to check if they are equal
            if (ISBN == bk.ISBN &&
                condition == bk.condition && 
                language == bk.language){
                
                return true;
            }
            return false;
        }

        friend ostream& operator<<(ostream& output, BookClass& bk);     // operator overloading for the << operator to output book objects easier


};

ostream& operator<<(ostream& os, BookClass& bk){    // operator overloading for the << operator to output book objects easier
    os << left << "ISBN: " << setw(3) << bk.ISBN << " | Condition: " << setw(7) << bk.condition << " | Language: " << setw(7) << bk.language;
    return os;
}

void storeObj (vector<BookClass>& vectorBooks, ifstream& inputFile, BookClass& currentBook);    //declarations

bool linearSearch(vector<BookClass>& newBooksVector, BookClass reqBook);    

bool binarySearch(vector<BookClass>& newBooksVector, BookClass reqBook);    

bool recursiveBSearch(vector<BookClass>& newBooksVector, BookClass reqBook, int low, int high);    


int main(int argc, char* argv[]) {  

    if(argc != 3 && argc != 4){   // check command line for 3 or 4 arguements
        cerr << "Usage: .SearchNewBooks <newBooks.dat> <request.dat> <result_file.dat>\n";  
        return 1;
    }

    BookClass book1;
    BookClass book2;
    vector<BookClass> newBooksVec;
    vector<BookClass> requestVec;

    ifstream inFile;           // open file stream
    inFile.open(string(argv[1]));
    if (!inFile.is_open()) {
        cerr << "Inpute File Not Open\n";
        return 1;
    }
    storeObj(newBooksVec, inFile, book1); // function that gets a vector and stores all the books from inFile
    inFile.close();

    inFile.open(string(argv[2]));
    if (!inFile.is_open()) {
        cerr << "Inpute File Not Open\n";
        return 1;
    }
    storeObj(requestVec, inFile, book1);    // stores all the books from inFile into a Vector
    inFile.close();

    char response;
    cout << endl << "Choice of search method ([l]inear, [b]inary, [r]ecursiveBinary)?\n";
    cin >> response;

    while(response != 'l' && response != 'b' && response != 'r'){   // check user input is valid
        cout << "Not Valid Choice \nChoice of search method ([l]inear, [b]inary, [r]ecursiveBinary)?\n";
        cin >> response;
    }

    int hits = 0;
    chrono::high_resolution_clock::time_point start;
    if(response == 'l'){   // if user chooses linear
        start = chrono::high_resolution_clock::now();
        for (size_t i = 0; i < requestVec.size(); i++){  // loop for every book in request file
            if (linearSearch(newBooksVec, requestVec[i])){   // if function returns true then increment hits
                hits++;
            }
        }
        auto end = chrono::high_resolution_clock::now();
        double elapsed_us = chrono::duration<double, std::micro>(end - start).count();   // calculates time used
        cout << "CPU time: " << elapsed_us << " microseconds\n";
    }

    else if(response == 'b'){   // same code as previous if only change is the function being called
        start = chrono::high_resolution_clock::now();
        sort(newBooksVec.begin(), newBooksVec.end());
        for (size_t i = 0; i < requestVec.size(); i++){
            if (binarySearch(newBooksVec, requestVec[i])){   // calls binary search and sees if it finds match
                hits++;
            }
        }
        auto end = chrono::high_resolution_clock::now();
        double elapsed_us = chrono::duration<double, std::micro>(end - start).count();
        cout << "CPU time: " << elapsed_us << " microseconds\n";
    }
    
    else{  // same as previous two if statements but recursive
        start = chrono::high_resolution_clock::now();
        sort(newBooksVec.begin(), newBooksVec.end());
        int high = newBooksVec.size();
        for (size_t i = 0; i < requestVec.size(); i++){
            if (recursiveBSearch(newBooksVec, requestVec[i], 0, high)){   // calls recursive search and returns whether there is match
                hits++;
            }
        }
        auto end = chrono::high_resolution_clock::now();
        double elapsed_us = chrono::duration<double, std::micro>(end - start).count();
        cout << "CPU time: " << elapsed_us << " microseconds\n";
    }
    ofstream outFile;    // opens the outFile which is where results are stored
    if (argv[3] != NULL){   // opens the 4th arguement if it isn't empty
        outFile.open(string(argv[3]));   
        if (!outFile.is_open()) {
            cerr << "Output File Not Open";
            return 1;
        }
    }
   
    else{   // if argv[3] is empty open default outFile
        outFile.open("found.dat");
        if (!outFile.is_open()) {
            cerr << "Output File Not Open";
            return 1;
        }
    }
    outFile << hits;   // print to output file how many hits
    outFile.close();
    return 0;

}

bool linearSearch(vector<BookClass>& newBooksVector, BookClass reqBook){

    for (size_t j = 0; j < newBooksVector.size(); j++){   // loop for amount of newBooks
            if (reqBook.getISBN() == newBooksVector[j].getISBN() &&   // compare each book until match or end of file
                reqBook.getLang() == newBooksVector[j].getLang() &&
                reqBook.getCon() == newBooksVector[j].getCon()){   
                return true;
            }
        }
    return false; // if end of file return no match
}

bool binarySearch(vector<BookClass>& newBooksVector, BookClass reqBook){
    int low = 0;
    int high = newBooksVector.size();

    while (low <= high){  // while everything has not been searched
        int mid = (low + high) / 2;   // calculate mid
        if (newBooksVector[mid] == reqBook){   // if match true
            return true;
        }

        else if (newBooksVector[mid] < reqBook){   // if not match but less, make new low
            low = mid + 1;
        }
        
        else{
            high = mid - 1; // make new high if vector book too high
        }
           
    }
    return false;   // if everything has been searched return false
}

bool recursiveBSearch(vector<BookClass>& newBooksVector, BookClass reqBook, int low, int high){
    if (low <= high){
        int mid = (low + high) / 2;   // calculate mid

        if (newBooksVector[mid] == reqBook){   // if match return true
            return true;
        }

        else if (newBooksVector[mid] < reqBook){   // if book being searched for is higher than mid 
            return recursiveBSearch(newBooksVector, reqBook, mid+1, high); // recursively call passing mid+1 as new low
        }

        else{   // if book being searched for is higher than mid 
            return recursiveBSearch(newBooksVector, reqBook, low, mid-1); // recursively call passing mid-1 as new high
        }

    }
    return false;   // if everything has been searched return false
}

void storeObj (vector<BookClass>& vectorBooks,
 ifstream& inputFile, BookClass& currentBook){
    string holder = "";
    while (getline(inputFile, holder)){   
        stringstream ss(holder);
        string substr;
        //parse ss on commas
        getline(ss, substr, ',');   
        int isbn = stoi(substr);
        getline(ss, substr, ',');
        string lang = substr;
        getline(ss, substr, ',');
        string cond = substr;
        // create book object from parsed line
        currentBook.assignBook(isbn, lang, cond); 
        // push back new object onto vector 
        vectorBooks.push_back(currentBook);}}

