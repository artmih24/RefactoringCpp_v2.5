#include <iostream>
#include <string>
#include <cmath>
using namespace std;

class Complex {
    public:
    double re, im;

    Complex(double re, double im) {
        this->re = re;
        this->im = im;
    }

    double GetRe() {return this->re;}
    double GetIm() {return this->im;}
    void SetRe(double re) {this->re = re;}
    void SetIm(double im) {this->im = im;}
};

double GetHypotenuse(double a, double b) {
    return sqrt(a * a + b * b);
}

double GetCatetus(double c, double a) {
    return sqrt(abs(c * c - a * a));
}

int main(int argc, char* argv[]) {
    int a = 5,
        b = 3,
        c = 4;
    Complex c1 = Complex(a, b);
    cout << c1.re << "+" << c1.im << "j\n";
    double d = GetHypotenuse(b, c),
        e = GetHypotenuse(c1.re, c1.im);
    system("pause");
}