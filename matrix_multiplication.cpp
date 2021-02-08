#include <stdlib.h>
#include <stdio.h>
#include "mpi.h"
#include <time.h>
#include <sys/time.h>


// DIMENSIONxDIMENSION matrices
#define DIMENSION 4

MPI_Status status;

double matrix_a[DIMENSION][DIMENSION],matrix_b[DIMENSION][DIMENSION],matrix_c[DIMENSION][DIMENSION];

int main(int argc, char **argv)
{
  int processes, ID, totalSlaveTasks, source, destination, rows, offset;

  struct timeval start, stop;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &ID);
  MPI_Comm_size(MPI_COMM_WORLD, &processes);

  totalSlaveTasks = processes - 1;

 if (ID == 0) {
    srand( time(NULL) );
    for (int i = 0; i<DIMENSION; i++) {
      for (int j = 0; j<DIMENSION; j++) {
        matrix_a[i][j]= rand()%10;
        matrix_b[i][j]= rand()%10;
      }
    }
	
  printf("\n\t\tMatrix Multiplication - MPI - 4x4\n");
    
    printf("\n 1st Matrix\n\n");
    for (int i = 0; i<DIMENSION; i++) {
      for (int j = 0; j<DIMENSION; j++) {
        printf("%.0f\t", matrix_a[i][j]);
      }
	    printf("\n");
    }

    printf("\n 2nd Matrix\n\n");
    for (int i = 0; i<DIMENSION; i++) {
      for (int j = 0; j<DIMENSION; j++) {
        printf("%.0f\t", matrix_b[i][j]);
      }
	    printf("\n");
    }

    rows = DIMENSION/totalSlaveTasks;
    offset = 0;

    for (destination=1; destination <= totalSlaveTasks; destination++)
    {
      MPI_Send(&offset, 1, MPI_INT, destination, 1, MPI_COMM_WORLD);
      MPI_Send(&rows, 1, MPI_INT, destination, 1, MPI_COMM_WORLD);
      MPI_Send(&matrix_a[offset][0], rows*DIMENSION, MPI_DOUBLE,destination,1, MPI_COMM_WORLD);
      MPI_Send(&matrix_b, DIMENSION*DIMENSION, MPI_DOUBLE, destination, 1, MPI_COMM_WORLD);
      offset = offset + rows;
    }

    for (int i = 1; i <= totalSlaveTasks; i++)
    {
      source = i;
      MPI_Recv(&offset, 1, MPI_INT, source, 2, MPI_COMM_WORLD, &status);
      MPI_Recv(&rows, 1, MPI_INT, source, 2, MPI_COMM_WORLD, &status);
      MPI_Recv(&matrix_c[offset][0], rows*DIMENSION, MPI_DOUBLE, source, 2, MPI_COMM_WORLD, &status);
    }

    printf("\nResult Matrix C = Matrix A * Matrix B:\n\n");
    for (int i = 0; i<DIMENSION; i++) {
      for (int j = 0; j<DIMENSION; j++)
        printf("%.0f\t", matrix_c[i][j]);
      printf ("\n");
    }
    printf ("\n");
  }

  if (ID > 0) {

    source = 0;
    MPI_Recv(&offset, 1, MPI_INT, source, 1, MPI_COMM_WORLD, &status);
    MPI_Recv(&rows, 1, MPI_INT, source, 1, MPI_COMM_WORLD, &status);
    MPI_Recv(&matrix_a, rows*DIMENSION, MPI_DOUBLE, source, 1, MPI_COMM_WORLD, &status);
    MPI_Recv(&matrix_b, DIMENSION*DIMENSION, MPI_DOUBLE, source, 1, MPI_COMM_WORLD, &status);

    for (int k = 0; k<DIMENSION; k++) {
      for (int i = 0; i<rows; i++) {
        matrix_c[i][k] = 0.0;
        for (int j = 0; j<DIMENSION; j++)
          matrix_c[i][k] = matrix_c[i][k] + matrix_a[i][j] * matrix_b[j][k];
      }
    }

    MPI_Send(&offset, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
    MPI_Send(&rows, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
    MPI_Send(&matrix_c, rows*DIMENSION, MPI_DOUBLE, 0, 2, MPI_COMM_WORLD);
  }

  MPI_Finalize();
}
