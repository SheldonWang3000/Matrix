#include <iostream>
#include <fstream>
#include <string>
#include <time.h>
#include <Windows.h>
#include <emmintrin.h>
using namespace std;

#define MIN(x, y) (x < y ? x : y)

void init_matrix(int **&A, int N)
{
	for (int i = 0; i < N; ++i)
	{
		for (int j = 0; j < N; ++j)
		{
			A[i][j] = 0;
		}
	}
}

void new_matrix(int **&A, int N)
{
	A = new int*[N];
	for (int i = 0; i < N; ++i)
	{
		A[i] = new int[N];
	}
	init_matrix(A, N);
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
	system("pause");
}

void copy_matrix(int **&A, int **&B, int N)
{
	for (int i = 0; i < N; ++i)
	{
		for (int j = 0; j < N; ++j)
		{
			B[i][j] = A[i][j];
		}
	}
}
void delete_matrix(int **&A, int N)
{
	for (int i = 0; i < N; ++i)
	{
		delete[] A[i];
	}
	delete[] A;
}

void M0(int **A, int **B, int **C, int N)
{
	init_matrix(C, N);
	for (int i = 0; i < N; ++i)
	{
		for (int j = 0; j < N; ++j)
		{
			for (int k = 0; k < N; ++k)
			{
				C[i][j] += A[i][k] * B[k][j];
			}
		}
	}
}

void M1(int **A, int **B, int **C, int N)
{
	init_matrix(C, N);
	for (int i = 0; i < N; ++i)
	{
		for (int j = 0; j < N; ++j)
		{
			for (int k = 0; k < N; ++k)
			{
				C[i][k] += A[i][j] * B[j][k];
			}
		}
	}
}

struct argsForThread
{
	int **A;
	int **B;
	int **C;
	int No;
	int N;
	int threadNum;
};

DWORD WINAPI workForThread(LPVOID param)
{
	argsForThread * args = (argsForThread *)param;
	int interval = ceil((double)args->N / args->threadNum);
	int N = args->N;
	int No = args->No;
	for (int i = 0; i < N; i++)
	{
		for (int j = No * interval; j < MIN((No + 1) * interval, N); j++)
		{
			for (int k = 0; k < args->N; k++)
			{
				args->C[i][k] += args->A[i][j] * args->B[j][k];
			}
		}
	}
	return 0;
}

void M2(int **A, int **B, int **C, int N)
{
	int threadNum = 5;
	init_matrix(C, N);
	HANDLE * threads = new HANDLE[threadNum];
	DWORD * threadsID = new DWORD[threadNum];
	argsForThread * args = new argsForThread[threadNum];
	for (int i = 0; i < threadNum; i++)
	{
		args[i].A = A;
		args[i].B = B;
		args[i].C = C;
		args[i].No = i;
		args[i].N = N;
		args[i].threadNum = threadNum;
		threads[i] = CreateThread(NULL, 0, workForThread, &args[i], 0, &threadsID[i]);
		if (threads[i] == NULL)
		{
			for (int j = 0; j < i; j++)
			{
				CloseHandle(threads[j]);
			}
			cout << "ERROR:create thread failed, stop execute!" << endl;
			return;
		}
	}

	WaitForMultipleObjects(threadNum, threads, TRUE, INFINITE);
	for (int i = 0; i < threadNum; i++)
	{
		if (threads[i] != NULL)
			CloseHandle(threads[i]);
	}
}

void M3(int **A, int **B, int **C, int N)
{
	int **tmp;
	new_matrix(tmp, N);
	init_matrix(C, N);
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
			for (int k = 0; k < N; ++k)
			{
				C[i][j] += A[i][k] * tmp[j][k];
			}
		}
	}
	delete_matrix(tmp, N);
}

void M4(int **A, int **B, int **C, int N)
{
	int **tmp;
	int block = 256;
	new_matrix(tmp, N);
	init_matrix(C, N);
	for (int i = 0; i < N; ++i)
	{
		for (int j = 0; j < N; ++j)
		{
			tmp[i][j] = B[j][i];
		}
	}
	for (int jj = 0; jj < N; jj += block)
	{
		for (int kk = 0; kk < N; kk += block)
		{
			for (int i = 0; i < N; ++i)
			{
				for (int j = jj; j < MIN(N, jj + block); ++j)
				{
					int sum = 0;
					for (int k = kk; k < MIN(N, kk + block); ++k)
					{
						sum += A[i][k] * tmp[j][k];
					}
					C[i][j] += sum;
				}
			}
		}
	}
	delete_matrix(tmp, N);
}
int main()
{
	int N_List[] = { 128, 256, 512, 1024, 2048 };
	//int N_List[] = { 2048 };
	int multipy_times[] = { 1, 8, 64, 512, 4096 };
	//int multipy_times[] = { 4096 };
	time_t start, stop;
	void(*f[5]) (int**, int**, int**, int);
	f[0] = M0;
	f[1] = M1;
	f[2] = M2;
	f[3] = M3;
	f[4] = M4;

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
		/*cout << "Testing the best blockNum of M4" << endl;
		int l[] = { 8, 16, 32, 64, 128, 256, 512, 1024 };
		for each (auto t in l)
		{
			int **AA, **BB, **CC;
			new_matrix(AA, N);
			copy_matrix(A, AA, N);
			new_matrix(BB, N);
			copy_matrix(B, BB, N);
			new_matrix(CC, N);
			copy_matrix(C, CC, N);

			start = clock();
			M4(AA, BB, CC, N, t);
			M4(CC, AA, BB, N, t);
			M4(BB, CC, AA, N, t);
			stop = clock();
			cout <<"blockNum:" << t << "   " << (stop - start) / (CLOCKS_PER_SEC / 1000) << "ms" << endl;
		}*/
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
					int **AA = nullptr;
					int **BB = nullptr;
					int **CC = nullptr;
					new_matrix(AA, N);
					copy_matrix(A, AA, N);
					new_matrix(BB, N);
					copy_matrix(B, BB, N);
					new_matrix(CC, N);
					copy_matrix(C, CC, N);
					func(AA, BB, CC, N);
					func(CC, AA, BB, N);
					func(BB, CC, AA, N);
					delete_matrix(AA, N);
					delete_matrix(BB, N);
					delete_matrix(CC, N);
				}
				stop = clock();
				cout << (stop - start) / (CLOCKS_PER_SEC / 1000) << "ms" << endl;
			}
		}
		delete_matrix(A, N);
		delete_matrix(B, N);
		delete_matrix(C, N);
	}
	system("pause");
	return 0;
}