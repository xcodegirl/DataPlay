// DataPlay.cpp : Defines the entry point for the application.
//

#include "DataPlay.h"

using namespace std;

#include <json/json.h>
#include <fstream>
#include <string>

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
   // ifs.open("data.json");

    Json::CharReaderBuilder builder;
    builder["collectComments"] = true;
    JSONCPP_STRING errs;
    if (!parseFromStream(builder, ifs, &root, &errs)) {
        std::cout << errs << std::endl;
        return EXIT_FAILURE;
    }
    std::cout << root << std::endl;
    return EXIT_SUCCESS;
}