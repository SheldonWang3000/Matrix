#include <time.h>
#include <Windows.h>
#include <fstream>
#include <iostream>
#include <iomanip>

#define MIN (1 << 11)
#define MAX (1 << 25)
#define MAXSTRIDE 32
#define MAXSIZE MAX/sizeof(int)
#define MINSIZE MIN/sizeof(int)

using namespace std;
int data[MAXSIZE];
//struct timeeval
//{
//    /* data */
//    long tv_sec; /*秒*/
//    long tv_usec; /*微秒*/
//};

void new_data()
{
    srand(time(0));
    for (int i = 0; i < MAXSIZE; ++i)
        data[i] = rand() % 10;
}

int loop(int size, int stride)
{
    register int result = 0.0;
    volatile int sink = 0.0;
    // int times = 0;
    // printf("%d %d\n", size, stride);
    for (int i = 0; i < size; i += stride)
    {
         result += data[i];
         // ++ times;
    }
    sink = result;
	return sink;
    // printf("T %d\n", times);
}

double get_readrate(int size, int stride)
{
	int t = 10;
    //struct timeeval start, end;
	LARGE_INTEGER start, end, tc;
    loop(size, stride);
	QueryPerformanceFrequency(&tc);
	QueryPerformanceCounter(&start);
    //gettimeofday(&start, NULL);
	for (int i = 0; i < t; ++ i)
		loop(size, stride);
	QueryPerformanceCounter(&end);
    //gettimeofday(&end, NULL);
    double set_size = (double)(size * sizeof(int) / stride) / (1024 * 1024);
    //double times = end.tv_sec - start.tv_sec + (end.tv_usec - start.tv_usec) / 1000000.0;
	double times = (end.QuadPart - start.QuadPart) * 1.0 / tc.QuadPart;
    //printf("%.10f %d\n", times, result);
	return set_size * t / times;
}
int main()
{
	new_data();
	ofstream file("cache.txt", ios::out);
    for (int size = MAXSIZE; size >= MINSIZE; size >>= 1)
	{
		//if (size == (MINSIZE << 1)) break;
        for (int stride = 1; stride <= MAXSTRIDE; ++stride)
        {
            double readrate = get_readrate(size, stride);
            int set = size * sizeof(int) / 1024;
            if (set >= 1024)
            {
                set = set / 1024;
                //printf("%.2f ", readrate);
				file << setprecision(2) << setiosflags(ios::fixed) << readrate << ' ';
                //printf("set size:%dMB stride:%d readrate:%.2fMB/S\n", set, stride, readrate);
            }
            else {
                //printf("%.2f ", readrate);
				file << setprecision(2) << setiosflags(ios::fixed) << readrate << ' ';
                //printf("set size:%dKB stride:%d readrate:%.2fMB/S\n", set, stride, readrate);
            }
        }
        //printf("\n");
		file << endl;
    }
	file.close();
	return 0;
}
