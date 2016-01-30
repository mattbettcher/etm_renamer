#include <iostream>
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <locale>
#include <cctype>
#include <stdio.h>

using namespace std;



std::string & ltrim(std::string & str)
{
	auto it2 = std::find_if(str.begin(), str.end(), [](char ch) { return !std::isspace<char>(ch, std::locale::classic()); });
	str.erase(str.begin(), it2);
	return str;
}

std::string & rtrim(std::string & str)
{
	auto it1 = std::find_if(str.rbegin(), str.rend(), [](char ch) { return !std::isspace<char>(ch, std::locale::classic()); });
	str.erase(it1.base(), str.end());
	return str;
}

std::string & trim(std::string & str)
{
	return ltrim(rtrim(str));
}

std::string trim_copy(std::string const & str)
{
	auto s = str;
	return ltrim(rtrim(s));
}

bool is_number(const std::string& s)
{
	std::string::const_iterator it = s.begin();
	while (it != s.end() && std::isdigit(*it)) ++it;
	return !s.empty() && it == s.end();
}

string getName(string str)
{
	std::string::const_iterator it = str.begin();
	while (it != str.end() && !isspace(*it)) ++it;
	++it;
	while (it != str.end() && !isspace(*it)) ++it;
	str.erase(it, str.end());
	return str;
}

bool is_file_exist(const char *fileName)
{
	std::ifstream infile(fileName);
	return infile.good();
}

struct pdf
{
	string name;
	string fileName;
};

int main()
{
	ifstream file("etm.txt");
	if (!file.good())
		return 1;

	string line;
	pdf names[2000];
	int index = 0;

	while (getline(file, line))
	{
		istringstream iss (line);

		string result;
		while (getline(iss, result, '\"'))
		{
			result = trim_copy(result);
			if (result == "," || result == "") {
				// comma separates items
			}
			else if (is_number(result))
			{
				// if the string is just a number skip it as well
			}
			else {
				//cout << result << endl;
				// decode here
				auto s = result;
				if (s.length() > 3)
					s = trim(s.erase(3, 1000));
				//cout << s << endl;

				if (s == "MWO" || s == "TB" || s == "TM" || s == "LO") {
					// manual name
					auto name = getName(result);
					names[index++].name = name;
				}
				if (is_number(s)) {
					if (names[index - 1].fileName != result)
							names[index - 1].fileName = result;
				}
			}
		}
	}

	for (int i = 0; i < index; i++)
	{
		if (is_file_exist(names[i].fileName.c_str()))
		{
			names[i].name += ".pdf";
			rename(names[i].fileName.c_str(), names[i].name.c_str());
			cout << names[i].fileName << " renamed to -> " << names[i].name << endl;
		}
		else {
			cout << names[i].fileName << " does not exist!" << endl;
		}
	}
	cout << endl << "Written by Matt Bettcher ;)" << endl;
	system("pause");
	return 0;
}