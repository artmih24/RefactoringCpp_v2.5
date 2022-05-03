#include <iostream>  
#include <cstdlib> // для system  
using namespace std;  
 
int add0(int a, int b) {  
	return a + b; 
}  
 
int getTen(int a) {  
	int new_var_a = a;  
	new_var_a = 10;  
	return new_var_a; 
}  
 
int add2(int a) {  
	int new_var_a = a;  
	new_var_a += 2;  
	return new_var_a; 
}  
 
int getFive() {  
	return 5; 
}  
 
void copy(int x) {  
	int new_var_x = x;  
	int m = new_var_x;  
	new_var_x = 22;  
	new_var_x = m; 
}  
 
int mul2(int a) {  
	int new_var_a = a;  
	new_var_a = 2;  
	return add(new_var_a, new_var_a); 
}  
 
int mul2v2(int a) {  
	int new_var_a = a;  
	int res = add(new_var_a, new_var_a);  
	new_var_a = 3;  
	return res; 
}  
 
int sqr(int a) {  
	return a * a; 
}  
 
int rec(int x) {  
	if (x == 1)  
		return 1;  
	else  
		return rec(x - 1); 
}  
 
int rec_ref(int x) {  
	if (x == 1)  
		return 1;  
	else  
		return rec(x - 1); 
}  
 
int rec_new(int x) {  
	if (x == 1)  
		return 1;  
	else  
		return rec_new(x - 1); 
}  
 
int rec1(int x) {  
	if (x == 1)  
		return 1;  
	else  
		return rec(x - 1); 
}  
 
int rec1_new(int x) {  
	if (x == 1)  
		return 1;  
	else  
		return rec(x - 1); 
}  
 
int rec2(int x, int y) {  
	int z = y;  
	if (x == 1)  
		return 1;  
	else  
		return rec2(x - 1, y); 
}  
 
int rec_func(int x) {  
	if (x == 1)  
		return 1;  
	else  
		return rec_func(x - 1); 
}  
 
int func(int x) {  
	if (x == 1)  
		return 1;  
	else  
		return rec_func(x - 1); 
}  
 
int add5(int a) {  
	int new_var_a = a;  
	new_var_a += 5;  
	return new_var_a; 
}  
 
int getThree(int a) {  
	int new_var_a = a;  
	new_var_a = 3;  
	return new_var_a; 
}  
 
int myFunc(int a, int b, int c) {  
	int new_var_b = b;  
	int new_var_a = a;  
	new_var_a = getThree(new_var_a);  
	new_var_b = add5(new_var_a);  
	new_var_b += c;  
	return func(new_var_a); 
}  
 
int add(int a, int b) {  
	int new_var_a = a;  
	new_var_a += b;  
	return new_var_a; 
}  
 
int mul(int a, int b) {  
	int new_var_a = a;  
	new_var_a = new_var_a * b;  
	return new_var_a; 
}  
 
int recursion(int a, int b) {  
	if (a == 0)  
		return b;  
	else  
		return recursion(a - 1, a); 
}  
 
int sum_a_b(int a, int b) {  
	return a + b; 
}  
 
int sum_a_b_c(int a, int b, int c) {  
	return sum_a_b(a, b) + c; 
}  
 
float GetSqr(float a) {  
	return a * a; 
}  
 
float GetMax(float a, float b) {  
	return(a > b) ? a : b; 
}  
 
float SomeBigFunc(float a, float b) {  
	float new_var_b = b;  
	float new_var_a = a; // метод принимает на вход new_var_a и new_var_b  
	new_var_a = GetSqr(new_var_a);  
	new_var_b += new_var_a; // ... //// некоторые действия с new_var_a и new_var_b //// ...  
	return GetMax(new_var_a, new_var_b); // если программист плохо знаком с // кодом тела метода, то ему сначала // может показаться, что он // возвращает максимальное значение // из тех, что были поданы на вход 
}  
 
int main() {  
	int a = 11, b = 4, c = 8, d = 6;  
	cout << sum_a_b_c(a, b, c) << endl;  
	cout << recursion(a, b) << endl;  
	system("pause");  
	return 0; 
}  
  
