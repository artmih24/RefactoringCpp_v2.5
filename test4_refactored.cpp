#include <iostream>  
 
class SomeClass {  
public:  
double a;  
SomeClass(double a) {  
this->a = a;  
}  
void FooA() {  
std::cout << "a = " << a << std::endl; 
} 
};  
 
class New_SomeClass {  
public:  
double b;  
New_SomeClass(double b) {  
this->b = b;  
}  
void FooB() {  
std::cout << "b = " << b << std::endl; 
} 
};  
 
int main() {  
SomeClass someObject = SomeClass(5);  
New_SomeClass New_someObject = New_SomeClass(3);  
someObject.FooA();  
New_someObject.FooB();  
system("pause"); 
}  
  
