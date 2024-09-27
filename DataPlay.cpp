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
#include <random>

/*
Reads specified json file into a data structure,
optionally avoiding duplicates in field such as "name".
Then, performs data access and algorithms, such as:
	- randomly selcting a subsset of unique elements
*/
int main(int argc, char* argv[])
{
	// 1. parse commandline
	std::string filename;
	std::string uniqueKey;
	std::string filterKey;
	int filterValue = 0;
	bool containsMatch = false;
	bool caseMatch = false;
	bool checkForDuplicates = !uniqueKey.empty();
	bool filter = false;

	// some options to set 
	int options = 1;
	for (int i = 1; i < argc; ++i) {
		std::string arg = argv[i];
		if (arg == "-file") {
			if (i + 1 < argc)
				filename = argv[++i];
			cout << "Importing data from " << filename << endl;
		}
		else if (arg == "-choices") {
			if (i + 1 < argc)
				options = std::stoi(argv[++i]);
			cout << "Selecting " << options << " random choices." << endl;
		}
		else if (arg == "-noduplicates") {
			checkForDuplicates = true;
			if (i + 1 < argc)
				uniqueKey = argv[++i];
			cout << "Avoiding duplicate keys " << uniqueKey << "." << endl;
		}
		else if (arg == "-ignorecase") {
			caseMatch = true;
		}
		else if (arg == "-contains") {
			containsMatch = true;
		}
		else if (arg == "-filter") {
			filter = true;
			if (i + 1 < argc)
				filterKey = argv[++i];
			if (i + 1 < argc)
				filterValue = std::stoi(argv[++i]);
			uniqueKey = argv[3];
			cout << "Filtering key " << filterKey << " = " << filterValue << endl;
		}		
	}
	if (filename.empty()) {
		cout << "Please specify file to parse. Using default Data\\data.json" << endl;
		filename = "..\\..\\..\\Data\\data.json";
	}

	// 2. Load json file into root value
	Json::Value root;
	std::ifstream ifs;
	ifs.open(filename);
	
	Json::CharReaderBuilder builder;
	builder["collectComments"] = true;
	JSONCPP_STRING errs;
	if (!parseFromStream(builder, ifs, &root, &errs)) {
		cout << errs << endl;
		return EXIT_FAILURE;
	}

	// 3. Iterate over json structure and insert into vector
	std::vector <Json::Value> items;

	for (Json::Value::iterator it = root.begin(); it != root.end(); ++it) {
		Json::Value newval = *it;
		bool found = false;

		if (filter && newval[filterKey].asInt() != filterValue) continue;

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
					cout << "DUPLICATE ENTRY:  " << newval[uniqueKey] << " " << val2[uniqueKey] << endl;
					found = true;
				}
			}
		}

		if (!found)
			items.push_back(*it);
	}

	// 4. Perform random selection
	Json::Value selection;
	if(options > items.size()) options = static_cast<int>(items.size());

	std::random_device rd; // Seed source for the random number engine
	std::mt19937 gen(rd()); // Mersenne Twister engine seeded with rd()

	for (int i = 0; i < options; i++) { 
		std::uniform_int_distribution<> distrib(0, static_cast<int>(items.size()) - 1);
		int index = distrib(gen);
		selection[i] = items[index];
		items.erase(items.begin() + index);
	}
	cout << selection << endl;
	return EXIT_SUCCESS;
}