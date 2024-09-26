// DataPlay.cpp : Defines the entry point for the application.
//

#include "DataPlay.h"
#include <cstdlib>

using namespace std;

#include <json/json.h>
#include <fstream>
#include <string>
#include <ctime>

/*
Reads specified json file into a data structure,
optionally avoiding duplicates in field "name"
Then, performs data access and algorithms, such as: 
    - randomly selcting a subsset of unique elements
*/
int main(int argc, char* argv[])
{
    Json::Value root;

    // 1. parse commandline and open file
    std::ifstream ifs;
    if(argc == 1){
        std::cout << "Please specify file to parse." << std::endl;
        return EXIT_FAILURE;
    }

    std::string filename = std::string(argv[1]);
    ifs.open(argv[1]);

    // 2. Load json file into root value
    Json::CharReaderBuilder builder;
    builder["collectComments"] = true;
    JSONCPP_STRING errs;
    if (!parseFromStream(builder, ifs, &root, &errs)) {
        std::cout << errs << std::endl;
        return EXIT_FAILURE;
    }

    // 3. Iterate over json structure and insert into vector
    std::vector <Json::Value> items;

    for (Json::Value::iterator it=root.begin(); it!=root.end(); ++it) {
        Json::Value newval = *it;
    bool found = false;
    bool checkForDuplicates = true;
    bool exactMatch = true;
    if(checkForDuplicates)
         for (std::vector<Json::Value>::iterator it2=items.begin(); it2!=items.end(); ++it2) {
        Json::Value val2 = *it2;
        if(val2["name"].asString()==newval["name"].asString() 
        || !exactMatch 
            && (val2["name"].asString().find(newval["name"].asString())!=std::string::npos || newval["name"].asString().find(val2["name"].asString())!=std::string::npos)){
        cout << newval["name"] << " " << val2["name"] <<std::endl;
        found = true;
        }
    }
    
    if(!found)
        items.push_back(*it);
    }   

    // 4. Perform random selection
    std::srand(static_cast<unsigned int>(std::time(nullptr))); // use current time as seed for random generator

    std::vector<std::string> menu;
    menu.push_back("Option A");
    menu.push_back("Option B");
    menu.push_back("Option C");

    Json::Value selection;
    for(std::vector<std::string>::iterator it=menu.begin(); it!=menu.end(); ++it){
        int index = static_cast<int>(static_cast<double>(rand() / (RAND_MAX + 1.0))*items.size());
        selection[*it] = items[index];
        items.erase(items.begin() + index);
    }
    std::cout << selection << std::endl;
    return EXIT_SUCCESS;
}