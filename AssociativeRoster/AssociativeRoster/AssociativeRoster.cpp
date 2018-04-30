#include <iostream>
#include <map>
#include <string>
#include <list>
#include <fstream>

void FillEntries(std::map<std::string, std::list<std::string>> &entries, const std::string file_name);
void RemoveDropouts(std::map<std::string, std::list<std::string>> &entries, const std::string file_name);
void PrintEntries(std::map<std::string, std::list<std::string>> &entries);
void AllCaps(std::string &course);
void RemoveFileHandle(std::string &file_name);

int main(int argc, char *argv[]) {
    if (argc <= 1) {
        std::cout << "Invalid amount of arguments.\n";
        std::cout << "Format follows: ./program_name cs1.txt cs2.txt cs3.txt cs4.txt dropouts.txt";
        std::cout << std::endl;
        exit(1);
    }

    std::map<std::string, std::list<std::string>> studentEntries;
    for (int i = 1; i < argc - 1; ++i) {
        FillEntries(studentEntries, argv[i]);
    }
    std::cout << "LIST BEFORE REMOVING DROPOUTS\n\n";
    PrintEntries(studentEntries);

    std::cout << "\nLIST AFTER REMOVING DROPOUTS\n\n";
    RemoveDropouts(studentEntries, argv[argc - 1]);
    PrintEntries(studentEntries);
}

void FillEntries(std::map<std::string, std::list<std::string>> &entries, const std::string file_name) {
    std::ifstream in_file(file_name);
    if (!in_file) {
        std::cout << "Encountered error opening the file.";
        std::cout << std::endl;
        exit(1);
    }
    std::string course = file_name;
    RemoveFileHandle(course);
    AllCaps(course);
    std::list<std::string> courses;
    courses.push_back(course);

    std::string first_name = "", last_name = "", full_name = "";
    while (in_file >> first_name >> last_name) {
        full_name = first_name + ' ' + last_name;
        std::pair<std::map<std::string, std::list<std::string>>::iterator, bool> ret;   //Creates a pair of template types <map iterator, bool>
        ret = entries.insert({ full_name, courses });   //Assigns ret the result of inserting a pair of type <string, list<string>
        if (!ret.second) {  //If the inserted pair exists in the map already
            entries[full_name].push_back(course);
        }
        else {  //If the inserted pair is not in the map.
            entries.insert({ full_name, courses });
        }
    }
    in_file.close();
}

void RemoveDropouts(std::map<std::string, std::list<std::string>>& entries, const std::string file_name) {
    std::ifstream in_file(file_name);

    std::string first_name = "", last_name = "", full_name = "";
    while (in_file >> first_name >> last_name) {
        full_name = first_name + ' ' + last_name;
        entries.erase(full_name);
    }
    in_file.close();
}


void PrintEntries(std::map<std::string, std::list<std::string>> &entries) {
    for (const auto &map : entries) {
        std::cout << map.first << ": ";
        for (const auto &list : map.second) {
            std::cout << list << ' ';
        }
        std::cout << std::endl;
    }
}
/*
Converts the contents of a string to all uppercase letters.
*/
void AllCaps(std::string &course) {
    for (int i = 0; i < course.size(); ++i) {
        course[i] = toupper(course[i]);
    }
}

/*
Takes a file name and removes the handle at the end.
When a period is found before the extension, the file name becomes a substring with all values before that period.
EXAMPLE:
------------------
BEFORE: file1.txt
^
AFTER:  file1
------------------
*/
void RemoveFileHandle(std::string &file_name) {
    int begin_pos = 0;
    size_t handle_pos = file_name.find_last_of('.');
    file_name = file_name.substr(begin_pos, handle_pos);
}