#include "stdio.h"
#include "stdlib.h"
#include "time.h"
#include <math.h>
 
#include "mpi.h"

#define AROW 3
#define ACOL 2
 
#define MAX_VALUE 10

int proc_map(int i, int size)
{
    size = size - 1;
    int r = (int) ceil( (double)AROW / (double)size);
    int proc = i / r;
    return proc + 1;
}

int main(int argc, char** argv)
{
    int size, rank, i, j;
    MPI_Status Stat;
 
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
 
    if (rank == 0)
    {
        int a[AROW][ACOL];
        int b[ACOL];
        int c[AROW];
 
        /* Generating Random Values for A & B Array*/
        srand(time(NULL));
        for (i=0;i<AROW;i++)
        {
            for (j=0;j<ACOL;j++)
            {
                if (i==0) b[j] = rand() % MAX_VALUE;
                a[i][j] = rand() % MAX_VALUE;
            }
        }
 
        /* Printing the Matrix*/
 
        printf("Matrix A :\n");
        for (i=0;i<AROW;i++)
        {
            for (j=0;j<ACOL;j++)
            {
                printf("%3d ", a[i][j]);
            }
            printf("\n");
        }
        printf("\nMatrix B :\n");
        for (i=0;i<ACOL;i++)
        {
            printf("%3d ", b[i]);
        }
        printf("\n\n");
 
        /* (1) Sending B Values to other processes */
        for (j=1;j<size;j++)
        {
            MPI_Send(b, ACOL, MPI_INTEGER, j, 99, MPI_COMM_WORLD);
        }
 
        /* (2) Sending Required A Values to specific process */
        for (i=0;i<AROW;i++)
        {
            int processor = proc_map(i, size);
            MPI_Send(a[i], ACOL, MPI_INTEGER, processor, (100*(i+1)), MPI_COMM_WORLD);
        }
 
        /* (3) Gathering the result from other processes*/
        for (i=0;i<AROW;i++)
        {
            int source_process = proc_map(i, size);
            MPI_Recv(&c[i], 1, MPI_INTEGER, source_process, i, MPI_COMM_WORLD, &Stat);
            printf("P%d : c[%d]\t= %d\n", rank, i, c[i]);
        }
    }
    else
    {
        int b[ACOL];
 
        /* (1) Each process get B Values from Master */
        MPI_Recv(b, ACOL, MPI_INTEGER, 0, 99, MPI_COMM_WORLD, &Stat);
 
        /* (2) Get Required A Values from Master then Compute the result */
        for (i=0;i<AROW;i++)
        {
            int processor = proc_map(i, size);
            if (rank == processor)
            {
                int buffer[ACOL];
                MPI_Recv(buffer, ACOL, MPI_INTEGER, 0, (100*(i+1)), MPI_COMM_WORLD, &Stat);
                int sum = 0;
                for (j=0;j<ACOL;j++)
                {
                    sum = sum + (buffer[j] * b[j] );
                }
                MPI_Send(&sum, 1, MPI_INTEGER, 0, i, MPI_COMM_WORLD);
            }
        }
    }
 
    MPI_Finalize();
    return 0;
}
