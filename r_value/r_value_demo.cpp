#include<iostream>

using namespace std;

void foo(int&& a)
{
    cout << "a: " << a << endl;
}


class Base
{
public:
    Base() {
        cout << "base constructor called" << endl;
    }

    Base(const Base& rhs) {
        cout << "lvalue base copy constructor called" << endl;
    }

    Base(Base&& rhs) {
        cout << "rvalue base copy constructor called" << endl;
    }
};

class Derived: public Base
{
public:
    Derived() {
        cout << "derived constructor called" << endl;
    }

    Derived(const Derived& rhs): Base(rhs) {
        cout << "lvalue derived copy constructor called" << endl;
    }

    Derived(Derived&& rhs): Base(std::move(rhs)) {
    //Derived(Derived&& rhs): Base(rhs) {
        cout << "rvalue derived copy constructor called" << endl;
    }
};

int main(int argc, char* argv[])
{
    // 1. demo one
    // int i = 0;
    //foo(i);

    // 2. demo two
    Derived a;
    cout << "define variable a" << endl;
    Derived&& b = std::move(a);
    return 0;
}
