#define _GNU_SOURCE
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/resource.h>
#include <sys/time.h>

#define N 1024
#define CHILDREN 16
#define sqrt_p 4
const int stride = N / sqrt_p;

pthread_t threads[CHILDREN];



static long int* A[N];
static long int* B[N];
static long int* C[N];

void read_matrix(long int* mat[N], FILE* f)
{
	for (int row=0; row < N; ++row) {
		for (int col=0; col < N; ++col) {
			fscanf(f, "%d", &mat[row][col]);	
		}
	}
}

void print_matrix(long int *mat[N])
{
	for (int row=0; row < N; ++row) {
		for (int col=0; col < N; ++col) {
			printf("%d ", mat[row][col]);
		}
		printf("\n");
	}
	printf("\n");
}

void * mult (void * pid) {
	long int id = (long int) pid;
	long int p_row = id / sqrt_p;
	long int p_col = id % sqrt_p;
	long int row_start = p_row * stride;
	long int col_start = p_col * stride;
	long int row_end = (p_row+1) * stride;
	long int col_end = (p_col+1) * stride;
	for (int row=row_start; row < row_end; ++row) {
		for (int col=col_start; col_end < N; ++col) {
			long int sum = 0;
			for (int j=0; j < N; ++j) {
				sum += A[row][j] * B[j][col];
			}
			C[row][col] = sum;
		}
	}
}

int main(int argc, char* argv[])
{
	if (argc < 2) {
		printf("Syntax: ./MatMult input.txt\n");
		exit(1);
	}
	for (long int i=0; i < N; ++i) {
		A[i] = malloc(N*sizeof(long int));	
		B[i] = malloc(N*sizeof(long int));	
		C[i] = malloc(N*sizeof(long int));	
	}
	FILE* in_file = fopen(argv[1], "r");
	if (!in_file) {
		printf("Error: Could not open file %s\n", argv[1]);
		exit(1);
	}
	read_matrix(A, in_file);
	read_matrix(B, in_file);
	//printf("A:\n");
	//print_matrix(A);
	//printf("B:\n");
	//print_matrix(B);

	struct rusage start;
	struct rusage end;
	getrusage(RUSAGE_SELF, &start);
	printf("Time: %d.%d\n", start.ru_utime.tv_sec, start.ru_utime.tv_usec);

	for (long int i=0; i < CHILDREN; i++){
	  	pthread_create(&threads[i], NULL, mult, (void*)i);
	}
	
	for (long int i=0; i < CHILDREN; i++){
	  	pthread_join(threads[i], NULL);
	}



	getrusage(RUSAGE_SELF, &end);
	printf("Seconds: %d\n", end.ru_utime.tv_sec-start.ru_utime.tv_sec);
	print_matrix(C);
	for (long int i=0; i < N; ++i) {
		free(A[i]);
		free(B[i]);
		free(C[i]);
	}
}
