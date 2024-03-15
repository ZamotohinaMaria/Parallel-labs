#include <iostream>
#include <fstream>
#include <stdio.h>
#include <conio.h>
#include <random>
#include <chrono>
#include <string>

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

void Creator(int n, int*** Mat, const string filename) {
	*Mat = new int* [n];
	fstream fout;
	fout.open(filename, ofstream::out | ofstream::trunc);
	fout << n << endl;
	for (int i = 0; i < n; i++)
	{
		(*Mat)[i] = new int[n];
	}

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++) 
		{
			(*Mat)[i][j] = rand() % (100 + 100 + 1) - 100;
			fout << (*Mat)[i][j] << ' ';
		}
		fout << endl;
	}
}

int** Reader(const string filename){
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

int** Multiplier(int n, int** Mat1, int** Mat2) {
	int** Mat_res = new int* [n];
	for (int i = 0; i < n; i++)
	{
		Mat_res[i] = new int[n];
	}

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
	srand(time(0));

	int** Mat1 = NULL;
	int** Mat2 = NULL;
	int** Mat_res = NULL;

	fstream fout;
	fout.open("times.txt", ofstream::out | ofstream::trunc);
	

	for (int n = 200; n <= 2000; n += 200)
	{
		cout << n << endl;

		for (int i = 1; i <= 10; i++)
		{
			cout << '\t' << i;
			Creator(n, &Mat1, string("1matrix/1_matrix") + to_string(n) + string(".txt"));
			/*PrintMat(n, Mat1);*/

			Creator(n, &Mat2, string("2matrix/2_matrix") + to_string(n) + string(".txt"));
			/*PrintMat(n, Mat2);*/

			auto start = chrono::steady_clock::now();
			Mat_res = Multiplier(n, Mat1, Mat2);
			auto end = chrono::steady_clock::now();

			fout << n << ';' << double(chrono::duration_cast<chrono::milliseconds>(end - start).count());
			Writer(n, Mat_res, string("res_matrix/res_matrix") + to_string(n) + string(".txt"));
			/*PrintMat(3, Mat_res);*/
			for (int i = 0; i < n; i++)
			{
				delete[] Mat1[i];
				delete[] Mat2[i];
				delete[] Mat_res[i];
			}

			delete[] Mat1;
			delete[] Mat2;
			delete[] Mat_res;

			Mat1 = NULL;
			Mat2 = NULL;
			Mat_res = NULL;
		}
		fout << endl;
		cout << endl;
	}

	
	return 0;
}