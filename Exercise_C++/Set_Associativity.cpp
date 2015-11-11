#include <Windows.h>
#include <iostream>
#include <iomanip>

using namespace std;

void Geteway(int* arr){
	const int rep = 1024 * 1024;
	int p = 16 * 64 / 4;

	for (int i = 0; i < 520; i++){
		arr[p]++;
		p += (16 * 64 / 4);
		if (p >= 1024 * 128) p = 16 * 64 / 4;
	}
	p = 16 * 64 / 4;
	
	for (int offset = 1; offset < 20; offset++){
		unsigned __int64 start = __rdtsc();
		for (int m = 1; m < offset; m++){
			arr[p]++;
			p += (16 * 64 / 4);
		}
		unsigned __int64 end = __rdtsc();
		double Get_time = (end - start) / offset;
		cout << "f is " << offset << endl;
		cout << "time is " << Get_time << endl;
	}
}

int main()
{
	int way_number = 1024 * 128;
	int* arr = new int[way_number];
	Geteway(arr);
	system("pause");
}