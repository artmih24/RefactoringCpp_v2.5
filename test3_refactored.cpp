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
" University, at the Faculty of " << faculty << ", \nat the Department of " << department << ", in the group " << group << endl; 
}  
 
void PrintStudInfo_Obj_Artem_firstName_lastName_middleName_university_faculty_department_group(MPEI_Stud Artem) {  
string firstName = Artem.firstName;  
string lastName = Artem.lastName;  
string middleName = Artem.middleName;  
string university = Artem.university;  
string faculty = Artem.faculty;  
string department = Artem.department;  
string group = Artem.group;  
cout << "Student " << lastName << " " << firstName << " " << middleName << "\nstudying at the " << university <<  
" University, at the Faculty of " << faculty << ", \nat the Department of " << department << ", in the group " << group << endl; 
}  
 
int main() {  
MPEI_Stud Artem = MPEI_Stud("Artem", "Bykov", "Mikhailovich", "IVTI", "VMSS", "A - 08m - 20");  
PrintStudInfo_Obj_Artem_firstName_lastName_middleName_university_faculty_department_group(Artem);  
system("pause"); 
}  
  
