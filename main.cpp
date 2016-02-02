#include <io.h>
#include <iostream>
#include <istream>
#include <fstream>
#include <direct.h>
#include <vector>
#include <string>
#include <regex>
#include <iomanip>

using namespace std;

struct etmInfo
{
	string filename;
	string description;
	string pubType;
	string pubName;
};

// returns a list of subdirectories
vector<string> getsub()
{
	char originalDirectory[_MAX_PATH];
	_getcwd(originalDirectory, _MAX_PATH);
	_finddata_t fileinfo;
	auto dirs = vector<string>();
	intptr_t handle = _findfirst("*", &fileinfo);
	if (handle == -1) {
		return dirs;
	}
	do {
		if (strcmp(fileinfo.name, ".") == 0 || strcmp(fileinfo.name, "..") == 0)
			continue;
		if (fileinfo.attrib & _A_SUBDIR)
			dirs.emplace_back(string(fileinfo.name) + "\\");
	} while (_findnext(handle, &fileinfo) == 0);

	_findclose(handle); // Close the stream
	_chdir(originalDirectory);
	return dirs;
}

// returns a list of all extracted ETM info
vector<etmInfo> parseETM(string filename)
{
	vector<etmInfo> etm;
	string str;

	ifstream t(filename);
	if (!t.good()) {
		return etm;
	}
	
	while (getline(t, str))
	{
		etmInfo info;
		// strip out the quotes!
		auto result = smatch{};
		regex rgx("\"([^\"]*)\"");
		if (regex_search(str, result, rgx)) {
			if (result[1].length() > 1) {
				// need to grab Pub Number here
				auto subResult = smatch{};
				regex rgx2(R"((LO|MWO|TB|TM|SB)\s*(\w|-|&)*\s*)");
				if (regex_search(str, subResult, rgx2)) {
					info.pubType = subResult[1].str();
					info.pubName = subResult[0].str();
				}
				info.description = result[1].str();
			}
			str = result.suffix().str();
		}
		if (regex_search(str, result, rgx)) {
			if (result[1].length() > 1)
				info.filename = result[1].str();
			str = result.suffix().str();
		}
		if (info.description.length() > 1)
			etm.emplace_back(info);
	}

	return etm;
}

vector<etmInfo> cleanup(vector<etmInfo> info)
{
	info.erase(begin(info), begin(info) + 3);
	for (auto i = 1; i < info.size(); i++) {
		if (info[i - 1].filename == info[i].filename) {
			info[i - 1].description.erase(info[i - 1].description.find_last_not_of(" \n\r\t") + 1) += info[i].description.erase(info[i].description.find_last_not_of(" \n\r\t") + 1);
			info.erase(info.begin() + i);
			i--;
		}
	}
	return info;
}

bool does_file_exist(const char *fileName)
{
	ifstream infile(fileName);
	return infile.good();
}

vector<etmInfo> renameFiles(vector<etmInfo> files, string dir)
{
	vector<etmInfo> notFound;

	for (auto const& f : files) {
		if (does_file_exist((dir + f.filename).c_str())) {
			rename((dir + f.filename).c_str(), (dir + f.pubName + ".pdf").c_str());
			cout << "\t" << (dir + f.filename) << " renamed to -> " << (dir + f.pubName + ".pdf") << endl;
		}
		else {
			notFound.emplace_back(f);
		}
	}
	return notFound;
}

int main(int argc, char* argv[])
{
	auto dirs = getsub();
	dirs.emplace(dirs.begin(), "");
	
	for (auto const& c : dirs) {
		auto info = parseETM(c + "etm.txt");
		if (info.size() > 0) {
			cout << "Processing -> " << c << endl;

			auto cleanInfo = cleanup(info);

			// write file
			ofstream o(c + "description.txt");
			o << " Old File\t  Pub Name\t\t\t\t   Description" << endl;
			for (auto const& c : cleanInfo)
				o << left << setw(12) << c.filename << left << setw(24) << c.pubName << c.description << endl;
			o.close();

			// rename pdfs
			auto notFound = renameFiles(cleanInfo, c);
		}
	}
	cout << endl << "Finished!" << endl;
	system("PAUSE");
	return 0;
}
