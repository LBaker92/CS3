//TITLE:    Lab 4: Roster Without Conflicts
//AUTHOR:   Logan Baker
//DATE:     02-05-2018
//DESC:     Recode the roster example we studied in class such that it lists the classes each student is enrolled in.

#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <string>
#include <cstdlib>
#include <algorithm>

void FillRoster(std::list<std::string> &roster, std::string file_name);
void AddCourse(std::list<std::string> &roster, std::string file_name);
void PrintRoster(const std::list<std::string> &roster);
void AllCaps(std::string &course);
void RemoveFileHandle(std::string &file_name);

int main(int argc, char *argv[]) {
    if (argc <= 1) {
        std::cout << "usage: " << argv[0] << " list of courses, dropouts last" << std::endl;
        exit(1);
    }

    /*
    Creates a list and fills it with student names.
    The list is transferred (using std::move) to the vector.
    */
    std::vector<std::list<std::string>> student_entries;
    for (int i = 1; i < argc - 1; ++i) {
        std::list<std::string> roster;
        FillRoster(roster, argv[i]);
        student_entries.push_back(roster);
    }

    /*
    All of the elements (student names) in the vector are spliced into a master list.
    Then, the master list is sorted, and duplicate student names are removed.
    Finally, the courses the student took are added after the student's name.
    */
    std::list<std::string> master_student_list;
    for (auto &e : student_entries) {
        master_student_list.splice(master_student_list.end(), e);
    }
    master_student_list.sort();
    master_student_list.unique();
    std::cout << "All students before removing dropouts.\n\n";
    PrintRoster(master_student_list);
    /*
    Prints the names of students who were dropouts.
    */
    std::list<std::string> dropouts;
    FillRoster(dropouts, argv[5]);  //argv[5] is dropouts.txt

    for (const auto &str : dropouts) {
        master_student_list.remove(str);
    }
    for (int i = 1; i < argc - 1; ++i) {
        AddCourse(master_student_list, argv[i]);
    }
    std::cout << "\nAll students, dropouts removed and classes added.\n\n";
    PrintRoster(master_student_list);
}

/*
Opens a list of student names from a file and adds them to the roster.
*/
void FillRoster(std::list<std::string> &roster, std::string file_name) {
    std::ifstream in_file(file_name);
    if (!in_file) {
        std::cout << "Error. File did not open properly." << std::endl;
        exit(1);
    }
    std::string first_name, last_name;
    while (in_file >> first_name >> last_name) {
        roster.push_back(first_name + ' ' + last_name);
    }
    in_file.close();
}

/*
Opens a list of student names and compares it to the roster.
If the student's name in the roster is the same name that is in the file, then we know that student took the course.
If the student took the course, that course is added after the student's name.
*/
void AddCourse(std::list<std::string> &roster, std::string file_name) {
    std::ifstream in_file(file_name);
    if (!in_file) {
        std::cout << "ERROR";
        exit(1);
    }

    std::string course_name = file_name;
    RemoveFileHandle(course_name);
    AllCaps(course_name);

    std::list<std::string> students;
    std::string first_name, last_name;
    while (in_file >> first_name >> last_name) {
        std::string full_name = first_name + ' ' + last_name;
        students.push_back(full_name);
    }
    students.sort();
    for (auto &e : roster) {
        std::string e_name = e.substr(0, e.find(" CS"));
        for (auto &s : students) {
            if (s == e_name) {
                e += ' ' + course_name;
            }
        }
    }
    in_file.close();
}

void PrintRoster(const std::list<std::string> &roster) {
    for (const auto &str : roster)
        std::cout << str << std::endl;
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
    int handle_pos = file_name.find_last_of('.');
    file_name = file_name.substr(begin_pos, handle_pos);
}