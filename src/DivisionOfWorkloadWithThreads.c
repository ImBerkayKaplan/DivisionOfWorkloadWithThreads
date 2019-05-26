#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <sys/time.h>
#define n 1200
#define m 100
#define p 500
#define MILLION 1E6

pthread_attr_t attr;
int MatrixA[n][m], MatrixB[m][p], **MatrixC1, NumThreads, Shift;

void InitMatrixA(int matrix[n][m]){ //Initialize Matrix A 
    int i, j;
    for(i=0; i < n; i++){
	for(j=0; j < m; j++){
	    matrix[i][j] = j-i+2;	
	}
    }
}

void InitMatrixB(int matrix[m][p]){ // Initialize Matrix B
    int i, j;
    for(i=0; i < m; i++){
	for(j=0; j < p; j++){
	    matrix[i][j] = i-j+1;
	}
    }
}

void InitMatrixC(int **matrix){ // Initialize Matrix B
    int i, j;
    for(i=0; i < n; i++){
	for(j=0; j < p; j++){
	    matrix[i][j] = 0;
	}
    }
}

void *MatrixMult(void *arg){ //Initialize Matrix C
	
    int id = *((int*) arg);
	int i, j, k;
	int StartPoint;
	int EndPoint;
	i = (n/NumThreads);
	StartPoint = i * id;
	EndPoint = StartPoint + i;

	if(Shift >= id + 1){
		StartPoint += id;
		EndPoint += id + 1;
	}else{ 
		StartPoint += Shift;
		EndPoint += Shift;
	}
	
	for(i= StartPoint; i < EndPoint; i++){
		for(j=0; j< p; j++){
			MatrixC1[i][j] = 0;
			for(k=0; k<m; k++){
			
				MatrixC1[i][j] += MatrixA[i][k]*MatrixB[k][j];

			}

		}

	}	
    return 0; 
}

void PrintMatrix(int **matrix){ //Print C1[i*25][i*25] for i=0,1,...,19 
	int i,j;
	for(i=0; i<20; i++){

		for(j = 0; j< 20; j++){
			
			printf("%d ", matrix[i * 25][j * 25]);

		}

	}

}

void CompareMatrix(int **MatC1, int **MatTest){//[n][p]){ //Compares two matrices of size [n][p]
	int i,j;

	for(i=0; i<n; i++){

		for(j=0; j< p; j++){

			if(MatC1[i][j] != MatTest[i][j]){
				printf("i = %d j = %d", i, j);
				printf("Error\n");
				exit(0);

			}
		}
	}
	printf("No Error\n");
}


int main(int argc, char *argv[]) {

        // Error checking for the number of arguments
        if(argc != 2){
	fprintf(stderr, "Please enter 1 argument.\nThe first positive integer argument is the number of threads that will work on multiplying the matrices.\n");
	exit(0);
        }

	int **Answer, i, j;
	pthread_attr_init(&attr);
	int IterateNumber = atoi(argv[1]);
	NumThreads = 1;
	InitMatrixA(MatrixA); //Initialize matrix A
	InitMatrixB(MatrixB); //Initialize matrix B

	MatrixC1 = (int**)malloc(n*sizeof(int*));  //Malloc Memory for Matix C1
	Answer = (int**)malloc(n*sizeof(int*));  //Malloc Memory for Matix C1
	for(i = 0; i < n; i++){
	    MatrixC1[i] = (int *)malloc(p*sizeof(int));
	    Answer[i] = (int *)malloc(p*sizeof(int));
	}
	
	
	// Initializing the threads
        pthread_t thread;
        struct timeval start, end;
      	gettimeofday(&start, NULL);
	int id = 0;
	Shift = 0;
	pthread_create(&thread, &attr, MatrixMult, &id);
	pthread_join(thread, NULL);
        gettimeofday(&end, NULL);

	// Calculate the result and print it
	printf("Theads Seconds\n");
	printf("1    %f\n", ((end.tv_sec * 1000000 + end.tv_usec)- (start.tv_sec * 1000000 + start.tv_usec))/MILLION);
	Answer = MatrixC1;

	// Multiply the matrices with every number of threads until the upper bound from the command line argument is reached
	int idMult[IterateNumber];
	for(i = 2; i <= IterateNumber; i++){
		
		InitMatrixC(MatrixC1);
		pthread_t threads[i];
		
        	NumThreads = i;
		// Start the clock
      		gettimeofday(&start, NULL);
		Shift = n % NumThreads;
		for(j = 0; j < i; j++){	
			idMult[j] = j;
			pthread_create(&threads[j], &attr, MatrixMult, &idMult[j]);
		}
		for(j = 0; j < i; j++){
			pthread_join(threads[j], NULL);
		}
		// Stop the clock
        	gettimeofday(&end, NULL);

		// Calculate the result and print it
		printf("%d    %f\n", NumThreads, ((end.tv_sec * 1000000 + end.tv_usec)- (start.tv_sec * 1000000 + start.tv_usec))/MILLION);
		//PrintMatrix(MatrixC1);
		CompareMatrix(Answer, MatrixC1);
		
	}



	//Ending the program by freeing the elements of the matrix
	for(i = 0; i < n; i++){  //Free matrix C1 
   	    free(MatrixC1[i]);
        }
	free(MatrixC1);
	exit(0);//exit program
}
