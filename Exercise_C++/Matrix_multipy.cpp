#include <iostream>
#include <fstream>
#include <string>
#include <time.h>
using namespace std;

void new_matrix(int **&A, int N)
{
	A = new int*[N];
	for (int i = 0; i < N; ++i)
	{
		A[i] = new int[N];
	}
	// cout << "test" << endl;
}

void output_matrix(int **&A, int N)
{
	for (int i = 0; i < N; ++i)
	{
		for (int j = 0; j < N; ++j)
		{
			cout << A[i][j] << ' ';
		}
		cout << endl;
	}
}

void M0(int **A, int **B, int **C, int N)
{
	for (int i = 0; i < N; ++i)
	{
		for (int j = 0; j < N; ++j)
		{
			int sum = 0;
			for (int k = 0; k < N; ++k)
			{
				sum += A[i][k] * B[k][j];
			}
			C[i][j] = sum;
		}
	}
}

void M1(int **A, int **B, int **C, int N)
{
	int **tmp;
	new_matrix(tmp, N);
	for (int i = 0; i < N; ++i)
	{
		for (int j = 0; j < N; ++j)
		{
			tmp[i][j] = B[j][i];
		}
	}
	for (int i = 0; i < N; ++i)
	{
		for (int j = 0; j < N; ++j)
		{
			int sum = 0;
			for (int k = 0; k < N; ++k)
			{
				sum += A[i][k] * B[j][k];
			}
			C[i][j] = sum;
		}
	}
}

int main()
{
	//int N_List[] = { 128, 256, 512, 1024, 2048 };
	int N_List[] = { 128 };
	int multipy_times[] = { 1, 8, 64, 512, 4096 };
	time_t start, stop;
	void(*f[2]) (int**, int**, int**, int);
	f[0] = M0;
	f[1] = M1;
	for each (auto N in N_List)
	{
		cout << "----------" << "This is " << N << " * " << N << " matrix" << "----------" << endl;
		int **A = nullptr; 
		int **B = nullptr; 
		int **C = nullptr; 
		new_matrix(A, N);
		new_matrix(B, N);
		new_matrix(C, N);
		string file_path = "matrix";
		file_path.append(to_string(N));
		file_path.append(".txt");
		fstream file;
		file.open(file_path, ios::in);
		for (int i = 0; i < N; ++i)
		{
			for (int j = 0; j < N; ++j)
			{
				int tmp = 0;
				file >> tmp;
				A[i][j] = tmp;
				B[i][j] = tmp;
				C[i][j] = tmp;
			}
		}
		file.close();
		int num = 0;
		for each (auto func in f)
		{
			cout << "	----------" << "This is using M" << num++ << "----------" << endl;
			for each (auto n in multipy_times)
			{
				cout << "		" << n << " times:";
				start = clock();
				for (int t = 0; t < n; ++t)
				{
					func(A, B, C, N);
					func(C, A, B, N);
					func(B, C, A, N);
				}
				stop = clock();
				cout << (stop - start) / (CLOCKS_PER_SEC / 1000) << "ms" << endl;
				
			}
		}
	}
	
}