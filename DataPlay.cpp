// DataPlay.cpp : Defines the entry point for the application.
//

#include "DataPlay.h"
#include <cstdlib>

using namespace std;

#include <json/json.h>
#include <fstream>
#include <string>
#include <algorithm>
#include <ctime>

/*
Reads specified json file into a data structure,
optionally avoiding duplicates in field such as "name".
Then, performs data access and algorithms, such as:
	- randomly selcting a subsset of unique elements
*/
int main(int argc, char* argv[])
{

	Json::Value root;
	std::string filename;

	// 1. parse commandline
	if (argc == 1) {
		std::cout << "Please specify file to parse. Using default Data\\data.json" << std::endl;
		filename = "..\\..\\..\\Data\\data.json";
	}
	else
	  filename = std::string(argv[1]);

	// some options to set 
	std::string uniqueKey;
	bool containsMatch = false;
	bool caseMatch = true;
	if (argc == 3) {
		uniqueKey = argv[2];
		cout << "Avoiding duplicate keys " << uniqueKey << "." << endl;
	}
	bool checkForDuplicates = !uniqueKey.empty();

	// 2. Load json file into root value
	std::ifstream ifs;
	ifs.open(filename);
	
	Json::CharReaderBuilder builder;
	builder["collectComments"] = true;
	JSONCPP_STRING errs;
	if (!parseFromStream(builder, ifs, &root, &errs)) {
		std::cout << errs << std::endl;
		return EXIT_FAILURE;
	}

	// 3. Iterate over json structure and insert into vector
	std::vector <Json::Value> items;

	for (Json::Value::iterator it = root.begin(); it != root.end(); ++it) {
		Json::Value newval = *it;
		bool found = false;

		if (checkForDuplicates && newval[uniqueKey].isNull()) {
			cout << "Key " << uniqueKey << " doesn't exist." << endl;
			checkForDuplicates = false;
		}
		if (checkForDuplicates) {
			std::string newstr = newval[uniqueKey].asString();
			if (caseMatch)
				std::transform(newstr.begin(), newstr.end(), newstr.begin(), std::tolower);

			for (std::vector<Json::Value>::iterator it2 = items.begin(); it2 != items.end(); ++it2) {
				Json::Value val2 = *it2;
				std::string str2 = val2[uniqueKey].asString();

				if (caseMatch)
					std::transform(str2.begin(), str2.end(), str2.begin(), std::tolower);

				if (str2 == newstr
					|| containsMatch 
					&& (str2.find(newstr) != std::string::npos || newstr.find(str2) != std::string::npos)) {
					cout << "DUPLICATE ENTRY:  " << newval[uniqueKey] << " " << val2[uniqueKey] << std::endl;
					found = true;
				}
			}
		}

		if (!found)
			items.push_back(*it);
	}

	// 4. Perform random selection
	std::srand(static_cast<unsigned int>(std::time(nullptr))); // use current time as seed for random generator
	auto rng = std::default_random_engine(std::time(nullptr)){};
	std::shuffle(items.begin(), items.end(), rng);

	Json::Value selection;
	for (std::vector<std::string>::iterator it = menu.begin(); it != menu.end(); ++it) {
		int index = static_cast<int>(static_cast<double>(rand() / (RAND_MAX + 1.0)) * items.size());
		selection[*it] = items[index];
		items.erase(items.begin() + index);
	}
	std::cout << selection << std::endl;
	return EXIT_SUCCESS;
}