#include <iostream>

class SomeClass
{
public:
    double a, b;

    SomeClass(double a, double b) {
        this->a = a;
        this->b = b;
    }

    void FooA() {
        std::cout << "a = " << a << std::endl;
    }

    void FooB() {
        std::cout << "b = " << b << std::endl;
    }
};

int main(int argc, char* argv[]) {
    SomeClass someObject = SomeClass(5, 3);
    someObject.FooA();
    someObject.FooB();
    system("pause");
}