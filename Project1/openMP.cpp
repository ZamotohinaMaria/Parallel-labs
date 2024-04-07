#include <iostream>
#include <fstream>
#include <stdio.h>
#include <conio.h>
#include <random>
#include <chrono>
#include <string>

#include <omp.h>

using namespace std;

void PrintMat(int n, int** Mat) {
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			cout << Mat[i][j] << '\t';
		}
		cout << endl;
	}
}

void Creator(int n, int*** Mat) {
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			(*Mat)[i][j] = rand() % (100 + 100 + 1) - 100;
		}
	}
}

int** Reader(const string filename) {
	fstream fin;
	fin.open(filename);

	int n;
	fin >> n;

	int** Mat = new int* [n];
	for (int i = 0; i < n; i++)
	{
		Mat[i] = new int[n];
	}

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++) {
			fin >> Mat[i][j];
		}
	}

	return Mat;
}

void Writer(int n, int** Mat, const string filename)
{
	fstream fout;
	fout.open(filename, ofstream::out | ofstream::trunc);
	fout << n << endl;

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			fout << Mat[i][j] << ' ';
		}
		fout << endl;
	}
}

int** Multiplier(int n, int** Mat1, int** Mat2, int threads_num) {
	int** Mat_res = new int* [n];
	for (int i = 0; i < n; i++)
	{
		Mat_res[i] = new int[n];
	}
	omp_set_num_threads(threads_num);
#pragma omp parallel for 
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			Mat_res[i][j] = 0;
			for (int k = 0; k < n; k++)
			{
				Mat_res[i][j] += Mat1[i][k] * Mat2[k][j];
			}
		}
	}
	return Mat_res;
}



int main() {
//	#ifdef _OPENMP
//		std::cout
//			<< "OpenMP Version: "
//			<< _OPENMP / 100 << " (" << _OPENMP % 100 << ")" << std::endl;
//	#else
//		std::cout << "Sequential Version" << std::endl;
//	#endif
//	std::cout
//		<< "Processors: " << omp_get_num_procs()
//		<< ", Max threads: " << omp_get_max_threads()
//		<< std::endl;
//
//	const int target_thread_num = 10;
//	omp_set_num_threads(target_thread_num);
//
//#pragma omp parallel 
//	cout << "OpenMP Test" << endl;

	srand(time(0));

	int** Mat1 = NULL;
	int** Mat2 = NULL;
	int** Mat_res = NULL;

	for (int threads_num = 5; threads_num <= 20; threads_num += 5)
	{
		cout << "\nNum of threats = " << threads_num << endl;
		fstream fout;
		fout.open(string("2lab/times") + to_string(threads_num) +".txt", ofstream::out | ofstream::trunc);
		double ti[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

		for (int n = 200; n <= 2000; n += 200)
		{
			cout << n << endl;


			Mat1 = new int* [n];
			Mat2 = new int* [n];
			Mat_res = new int* [n];

			for (int i = 0; i < n; i++)
			{
				Mat_res[i] = new int[n];
				Mat1[i] = new int[n];
				Mat2[i] = new int[n];
			}
			for (int i = 1; i <= 10; i++)
			{
				cout << '\t' << i;
				Creator(n, &Mat1);
				Creator(n, &Mat2);

				auto start = chrono::steady_clock::now();
				Mat_res = Multiplier(n, Mat1, Mat2, threads_num);
				auto end = chrono::steady_clock::now();

				ti[i - 1] = double(chrono::duration_cast<chrono::milliseconds>(end - start).count());
			}

			Writer(n, Mat_res, string("2lab/res_matrix/res_matrix") + to_string(n) + string(".txt"));
			Writer(n, Mat1, string("2lab/1matrix/1_matrix") + to_string(n) + string(".txt"));
			Writer(n, Mat2, string("2lab/2matrix/2_matrix") + to_string(n) + string(".txt"));
			fout << n << ';';
			for (int i = 0; i < 10; i++)
			{
				fout << ti[i] << ';';
				cout << ti[i] << ';';
			}
			fout << endl;
			cout << endl;

			for (int i = 0; i < n; i++)
			{
				delete[] Mat_res[i];
				delete[] Mat1[i];
				delete[] Mat2[i];
			}

			delete[] Mat_res;
			delete[] Mat1;
			delete[] Mat2;

			Mat_res = NULL;
			Mat1 = NULL;
			Mat2 = NULL;

		}

		fout.close();
	}

	return 0;
}