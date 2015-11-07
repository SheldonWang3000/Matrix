#include <iostream>

using namespace std;

void f1()
{
	cout << "f1" << endl;
}

void f2()
{
	cout << "f2" << endl;
}

int main()
{
	void (*k[2])();
	k[0] = f1;
	k[0]();
	k[1] = f2;
	k[1]();
	cout << "a" << "B" << endl;
}