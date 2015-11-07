#include <iostream>
#include <fstream>
#include <string>
#include <time.h>

using namespace std;

int main()
{
	int N_List[] = { 128, 256, 512, 1024, 2048 };
	for each (auto N in N_List)
	{
		ofstream file;
		string file_path;
		file_path = "matrix";
		file_path.append(to_string(N));
		file_path.append(".txt");
		file.open(file_path, ios::out);
		srand(time(0));
		for (int i = 0; i < N; ++i)
		{
			for (int j = 0; j < N; ++j)
			{
				int tmp = rand() % 10;
				file << tmp << ' ';
			}
			file << endl;
		}
		file.close();
	}
	cout << "Done!" << endl;
	system("pause");
	return 0;
}