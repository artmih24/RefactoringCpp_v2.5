#include <iostream>
#include <cstdlib> // для system
using namespace std;

int add0(int a, int b) {
	return a + b;
}

int getTen(int a) {
	a = 10;
	return a;
}

int add2(int a) {
	a += 2;
	return a;
}

int getFive(int a) {
	return 5;
}

void copy(int x) {
	int m = x;
	x = 22;
	x = m;
}

int mul2(int a) {
	a = 2;
	return add(a, a);
}

int mul2v2(int a) {
	int res = add(a, a);
	a = 3;
	return res;
}

int sqr(int a, int b) {
	return a * a;
}

int rec(int x, int y) {
	if (x == 1)
		return 1;
	else
		return rec(x - 1, y);
}

int rec_ref(int x, int y) {   
	if (x == 1)  
		return 1;  
	else  
		return rec(x - 1, y); 
}  

int rec_new(int x, int y) {   
	if (x == 1)  
		return 1;  
	else  
		return rec_new(x - 1, y); 
}  

int rec1(int x, int y) {
	if (x == 1)
		return 1;
	else
		return rec(x - 1, y + 1);
} 

int rec1_new(int x, int y) {  
	if (x == 1)  
		return 1;  
	else  
		return rec(x - 1, y); 
}  

int rec2(int x, int y) {
	int z = y;
	if (x == 1)
		return 1;
	else
		return rec2(x - 1, y);
}

int rec_func(int x, int y) {
	if (x == 1)
		return 1;
	else
		return rec_func(x - 1, y);
}

int func(int x, int y) {
	if (x == 1)
		return 1;
	else
		return rec_func(x - 1, y + 1);
} 

int add5(int a) {
	a += 5;
	return a;
}

int getThree(int a) {
	a = 3;
	return a;
}

int myFunc(int a, int b, int c) {
	a = getThree(a);
	b = add5(a);
	b += c;
	return func(a, b);
}

int add(int a, int b) {
	a += b;
	return a;
}

int mul(int a, int b) {
	a = a * b;
	return a;
}

int recursion(int a, int b, int c, int d) {
	if (a == 0)
		return b;
	else
		return recursion(a - 1, a, 1, b);
} 

int sum_a_b(int a, int b, int c, int d) {
	return a + b;
}

int sum_a_b_c(int a, int b, int c, int d) {
	return sum_a_b(a, b, c, d) + c;
}

float GetSqr(float a) {
	return a * a;
}

float GetMax(float a, float b) {
	return (a > b) ? a : b;
}
 
float SomeBigFunc(float a, float b) {
	// метод принимает на вход a и b
	a = GetSqr(a);
	b += a;
	// ...
	//
	// некоторые действия с a и b
	//
	// ...
	return GetMax(a, b);
	// если программист плохо знаком с
	// кодом тела метода, то ему сначала
	// может показаться, что он 
	// возвращает максимальное значение 
	// из тех, что были поданы на вход
}

int main(int argc, char* argv[]) { 
	int a = 11, b = 4, c = 8, d = 6;
	cout << sum_a_b_c(a, b, c, d) << endl;
	cout << recursion(a, b, c, d) << endl;
	system("pause");
	return 0; 
}