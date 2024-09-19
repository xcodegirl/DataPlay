// DataPlay.cpp : Defines the entry point for the application.
//

#include "DataPlay.h"
#include <cstdlib>

using namespace std;

#include <json/json.h>
#include <fstream>
#include <string>
#include <ctime>

#include <google/cloud/storage/client.h>

int main(int argc, char* argv[])
{
    Json::Value root;
    std::ifstream ifs;
    std::string filename = std::string(argv[1]);
    if(!(filename.empty()))
        ifs.open(argv[1]);
    else {
        std::cout << "Please specify file to parse." << std::endl;
        return EXIT_FAILURE;
    }

    Json::CharReaderBuilder builder;
    builder["collectComments"] = true;
    JSONCPP_STRING errs;
    if (!parseFromStream(builder, ifs, &root, &errs)) {
        std::cout << errs << std::endl;
        return EXIT_FAILURE;
    }

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

    std::srand(std::time(nullptr)); // use current time as seed for random generator

    std::vector<std::string> menu;
    menu.push_back("Monday");
    menu.push_back("Tuesday");
    menu.push_back("Wednesday");
    menu.push_back("Thursday");
    menu.push_back("Friday");
    menu.push_back("Saturday");
    menu.push_back("Sunday");

    Json::Value week;
    for(std::vector<std::string>::iterator it=menu.begin(); it!=menu.end(); ++it){
        int index = static_cast<int>(static_cast<double>(rand() / (RAND_MAX + 1.0))*items.size());
        week[*it] = items[index];
        items.erase(items.begin() + index);
    }
    std::cout << week << std::endl;
  // Prompt: Every time I ask, give one unique meal idea similar to a meal-in-a-box subscription service, formatted in json, with recipes for each component

    //Prompt: Pick 20 word puzzle theme ideas. Pick 5 common words for each theme, make longer than 2 letters and eliminate redundant terms. Give each theme a difficulty rating from 1 being most easy to 10 being most difficult. Format into json result.
    
    return EXIT_SUCCESS;
}