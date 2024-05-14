#include <iostream>
#include <fstream>
#include <stdio.h>
#include <conio.h>
#include <random>
#include <chrono>
#include <string>
#include<windows.h>

#include "mpi.h"

using namespace std;

#define MASTER_TO_SLAVE_TAG 1 
#define SLAVE_TO_MASTER_TAG 4

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

int** Reader(const std::string filename) {
	std::fstream fin;
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

void Writer(int n, int** Mat, const std::string filename)
{
	std::fstream fout;
	fout.open(filename, std::ofstream::out | std::ofstream::trunc);
	fout << n << std::endl;

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			fout << Mat[i][j] << ' ';
		}
		fout << std::endl;
	}
}

int rank; //process rank
int size; //number of processes
int i, j, k; //helper variables
int low_bound; //low bound of the number of rows of [A] allocated to a slave
int upper_bound; //upper bound of the number of rows of [A] allocated to a slave
int portion; //portion of the number of rows of [A] allocated to a slave
MPI_Status status; // store status of a MPI_Recv
MPI_Request request; //capture request of a MPI_Isend

//int** Multiplier(int n, int** Mat1, int** Mat2, int argc, char* argv[]) {
//	int** Mat_res = new int* [n];
//	for (int i = 0; i < n; i++)
//	{
//		Mat_res[i] = new int[n];
//	}
//
//
//	/* master initializes work*/
//	if (rank == 0) {
//		for (i = 1; i < size; i++) {//for each slave other than the master
//			portion = (n / (size - 1)); // calculate portion without master
//			low_bound = (i - 1) * portion;
//			if (((i + 1) == size) && ((n % (size - 1)) != 0)) {//if rows of [A] cannot be equally divided among slaves
//				upper_bound = n; //last slave gets all the remaining rows
//			}
//			else {
//				upper_bound = low_bound + portion; //rows of [A] are equally divisable among slaves
//			}
//			send the low bound first without blocking, to the intended slave
//			MPI_Isend(&low_bound, 1, MPI_INT, i, MASTER_TO_SLAVE_TAG, MPI_COMM_WORLD, &request);
//			next send the upper bound without blocking, to the intended slave
//			MPI_Isend(&upper_bound, 1, MPI_INT, i, MASTER_TO_SLAVE_TAG + 1, MPI_COMM_WORLD, &request);
//			finally send the allocated row portion of [A] without blocking, to the intended slave
//			MPI_Isend(&Mat1[low_bound][0], (upper_bound - low_bound) * n, MPI_DOUBLE, i, MASTER_TO_SLAVE_TAG + 2, MPI_COMM_WORLD, &request);
//		}
//	}
//	broadcast [B] to all the slaves
//	MPI_Bcast(&Mat2, n * n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
//
//	/* work done by slaves*/
//	if (rank > 0) {
//		receive low bound from the master
//		MPI_Recv(&low_bound, 1, MPI_INT, 0, MASTER_TO_SLAVE_TAG, MPI_COMM_WORLD, &status);
//		next receive upper bound from the master
//		MPI_Recv(&upper_bound, 1, MPI_INT, 0, MASTER_TO_SLAVE_TAG + 1, MPI_COMM_WORLD, &status);
//		finally receive row portion of [A] to be processed from the master
//		MPI_Recv(&Mat1[low_bound][0], (upper_bound - low_bound) * n, MPI_DOUBLE, 0, MASTER_TO_SLAVE_TAG + 2, MPI_COMM_WORLD, &status);
//		for (i = low_bound; i < upper_bound; i++) {//iterate through a given set of rows of [A]
//			for (j = 0; j < n; j++) {//iterate through columns of [B]
//				for (k = 0; k < n; k++) {//iterate through rows of [B]
//					Mat_res[i][j] += (Mat1[i][k] * Mat1[k][j]);
//				}
//			}
//		}
//		send back the low bound first without blocking, to the master
//		MPI_Isend(&low_bound, 1, MPI_INT, 0, SLAVE_TO_MASTER_TAG, MPI_COMM_WORLD, &request);
//		send the upper bound next without blocking, to the master
//		MPI_Isend(&upper_bound, 1, MPI_INT, 0, SLAVE_TO_MASTER_TAG + 1, MPI_COMM_WORLD, &request);
//		finally send the processed portion of data without blocking, to the master
//		MPI_Isend(&Mat_res[low_bound][0], (upper_bound - low_bound) * n, MPI_DOUBLE, 0, SLAVE_TO_MASTER_TAG + 2, MPI_COMM_WORLD, &request);
//	}
//
//	/* master gathers processed work*/
//	if (rank == 0) {
//		for (i = 1; i < size; i++) {// untill all slaves have handed back the processed data
//			receive low bound from a slave
//			MPI_Recv(&low_bound, 1, MPI_INT, i, SLAVE_TO_MASTER_TAG, MPI_COMM_WORLD, &status);
//			receive upper bound from a slave
//			MPI_Recv(&upper_bound, 1, MPI_INT, i, SLAVE_TO_MASTER_TAG + 1, MPI_COMM_WORLD, &status);
//			receive processed data from a slave
//			MPI_Recv(&Mat_res[low_bound][0], (upper_bound - low_bound) * n, MPI_DOUBLE, i, SLAVE_TO_MASTER_TAG + 2, MPI_COMM_WORLD, &status);
//		}
//	}
//	MPI_Finalize(); //finalize MPI operations
//	return Mat_res;
//}


int** resize_matrix(int size)
{
	int** matrix = new int* [size];
	for (int i = 0; i < size; ++i)
		matrix[i] = new int[size];
	return matrix;
}

int main(int argc, char* argv[]) {

	srand(time(0));

	CreateDirectory(L"res_matrix", NULL);
	CreateDirectory(L"1_matrix", NULL);
	CreateDirectory(L"2_matrix", NULL);
	CreateDirectory(L"times", NULL);

	int** Mat1 = NULL;
	int** Mat2 = NULL;
	int** Mat_res = NULL;

	int rank, size;
	double ti[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	std::fstream fout;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	if (rank == 0)
	{
		fout.open(std::string("times/time_") + std::to_string(size) + std::string(".txt"), std::ofstream::out | std::ofstream::trunc);
		std::cout << "size = " << size << endl;
	}

	for (int n = 200; n <= 2000; n += 200)
	{
		if (rank == 0)
		{
			
			std::cout << n << std::endl;
		}
		Mat1 = resize_matrix(n);
		Mat2 = resize_matrix(n);
		Mat_res = resize_matrix(n);

		MPI_Barrier(MPI_COMM_WORLD);
		double wtime = MPI_Wtime();

		for (int t = 1; t <= 10; t++)
		{
			if (rank == 0)
			{
				Creator(n, &Mat1);
				Creator(n, &Mat2);
				//std::cout << '\t' << i;
			}
			MPI_Bcast(&(Mat2[0][0]), n * n, MPI_INT, 0, MPI_COMM_WORLD);

			int rows_per_process = n / size;
			int rows_remaining = n % size;
			int start_row = rank * rows_per_process + min(rank, rows_remaining);
			int end_row = start_row + rows_per_process + (rank < rows_remaining ? 1 : 0);

			for (int i = start_row; i < end_row; i++)
			{
				for (int j = 0; j < n; j++)
				{
					Mat_res[i][j] = 0;
					for (int k = 0; k < n; k++)
						Mat_res[i][j] += Mat2[i][k] * Mat1[k][j];
				}
			}

			MPI_Barrier(MPI_COMM_WORLD);
			if (rank == 0)
				ti[t - 1] = MPI_Wtime() - wtime;
		}

		MPI_Barrier(MPI_COMM_WORLD);

		if (rank == 0)
		{
			Writer(n, Mat_res, std::string("res_matrix/res_matrix") + std::to_string(n) + std::string(".txt"));
			Writer(n, Mat1, std::string("1_matrix/1_matrix") + std::to_string(n) + std::string(".txt"));
			Writer(n, Mat2, std::string("2_matrix/2_matrix") + std::to_string(n) + std::string(".txt"));
			fout << n << ';';
			for (int i = 0; i < 10; i++)
			{
				fout << ti[i] << ';';
				std::cout << ti[i] << ';';
			}
			fout << std::endl;
			std::cout << std::endl;

		}
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

	MPI_Finalize();
	fout.close();


	return 0;
}