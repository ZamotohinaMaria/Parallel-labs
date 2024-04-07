//#include <iostream>
//#include <fstream>
//#include <stdio.h>
//#include <conio.h>
//#include <random>
//#include <chrono>
//#include <string>
//
//using namespace std;
//
//void PrintMat(int n, int** Mat) {
//	for (int i = 0; i < n; i++)
//	{
//		for (int j = 0; j < n; j++) 
//		{
//			cout << Mat[i][j] << '\t';
//		}
//		cout << endl;
//	}
//}
//
//void Creator(int n, int*** Mat) {
//	for (int i = 0; i < n; i++)
//	{
//		for (int j = 0; j < n; j++) 
//		{
//			(*Mat)[i][j] = rand() % (100 + 100 + 1) - 100;
//		}
//	}
//}
//
//int** Reader(const string filename){
//	fstream fin;
//	fin.open(filename);
//
//	int n;
//	fin >> n;
//
//	int** Mat = new int* [n];
//	for (int i = 0; i < n; i++)
//	{
//		Mat[i] = new int[n];
//	}
//
//	for (int i = 0; i < n; i++)
//	{
//		for (int j = 0; j < n; j++) {
//			fin >> Mat[i][j];
//		}
//	}
//
//	return Mat;
//}
//
//void Writer(int n, int** Mat, const string filename)
//{
//	fstream fout;
//	fout.open(filename, ofstream::out | ofstream::trunc);
//	fout << n << endl;
//
//	for (int i = 0; i < n; i++)
//	{
//		for (int j = 0; j < n; j++)
//		{
//			fout << Mat[i][j] << ' ';
//		}
//		fout << endl;
//	}
//}
//
//int** Multiplier(int n, int** Mat1, int** Mat2) {
//	int** Mat_res = new int* [n];
//	for (int i = 0; i < n; i++)
//	{
//		Mat_res[i] = new int[n];
//	}
//
//	for (int i = 0; i < n; i++)
//	{
//		for (int j = 0; j < n; j++)
//		{
//			Mat_res[i][j] = 0;
//			for (int k = 0; k < n; k++)
//			{
//				Mat_res[i][j] += Mat1[i][k] * Mat2[k][j];
//			}
//		}
//	}
//	return Mat_res;
//}
//
//
//
//int main() {
//	srand(time(0));
//
//	int** Mat1 = NULL;
//	int** Mat2 = NULL;
//	int** Mat_res = NULL;
//
//	fstream fout;
//	fout.open("1lab/times.txt", ofstream::out | ofstream::trunc);
//	double ti[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
//
//	for (int n = 200; n <= 2000; n += 200)
//	{
//		cout << n << endl;
//
//
//		Mat1 = new int* [n];
//		Mat2 = new int* [n];
//		Mat_res = new int* [n];
//
//		for (int i = 0; i < n; i++)
//		{
//			Mat_res[i] = new int[n];
//			Mat1[i] = new int[n];
//			Mat2[i] = new int[n];
//		}
//		for (int i = 1; i <= 10; i++)
//		{
//			cout << '\t' << i;
//			Creator(n, &Mat1);
//			Creator(n, &Mat2);
//
//			auto start = chrono::steady_clock::now();
//			Mat_res = Multiplier(n, Mat1, Mat2);
//			auto end = chrono::steady_clock::now();
//
//			ti[i -1] = double(chrono::duration_cast<chrono::milliseconds>(end - start).count());
//		}
//
//		Writer(n, Mat_res, string("1lab/res_matrix/res_matrix") + to_string(n) + string(".txt"));
//		Writer(n, Mat1, string("1lab/1matrix/1_matrix") + to_string(n) + string(".txt"));
//		Writer(n, Mat2, string("1lab/2matrix/2_matrix") + to_string(n) + string(".txt"));
//		fout << n << ';';
//		for (int i = 0; i < 10; i++)
//			fout << ti[i] << ';';
//		fout << endl;
//		cout << endl;
//	}
//
//	
//	return 0;
//}