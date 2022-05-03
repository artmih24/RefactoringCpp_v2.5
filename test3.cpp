#include <iostream>
#include <string>
using namespace std;

class MPEI_Stud {
    public:
    string firstName,
           lastName,
           middleName,
           university,
           faculty,
           department,
           group;

    MPEI_Stud(string firstName, string lastName, string middleName, string faculty, string department, string group) {
        this->firstName = firstName;
        this->lastName = lastName;
        this->middleName = middleName;
        this->university = "MPEI";
        this->faculty = faculty;
        this->department = department;
        this->group = group;
    }
};

void PrintStudInfo(string firstName, string lastName, string middleName, string university, string faculty, string department, string group) {
    cout << "Student " << lastName << " " << firstName << " " << middleName << "\nstudying at the " << university << 
        " University, at the Faculty of " << faculty << ",\nat the Department of " << department << ", in the group " << group << endl;
}

int main(int argc, char* argv[]) {
    MPEI_Stud Artem = MPEI_Stud("Artem", "Bykov", "Mikhailovich", "IVTI", "VMSS", "A-08m-20");
    PrintStudInfo(Artem.firstName, Artem.lastName, Artem.middleName, Artem.university, Artem.faculty, Artem.department, Artem.group);
    system("pause");
}