#include <stdio.h>
#include <string.h>
#include "mpi.h"

main(int argc, char *argv[])
{
	int my_rank;
	int numtasks;
	int source;
	int dest;
	int tag=0;
	static int a[10],b[10],i,j,k,l;
	for(i=0;i<10;i++)
	{
		a[i]=0;
		b[i]=0;
	}
	a[3]=1;
	a[2]=1;
	a[1]=1;
	b[1]=1;
	b[0]=1;
	MPI_Status status;
	MPI_Init(&argc,&argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	int sum=0;

	if(my_rank!=10)
	{
		sum=0;
		dest=10;
		for(i=0;i<10;i++)
		{
			for(j=0;j<10;j++)
			{
				if(i+j==my_rank)
				{
					sum=sum+a[i]*b[j];
				}
			}
		}
		MPI_Send(&sum,1,MPI_INT,dest,tag,MPI_COMM_WORLD);
	}

	else
	{
		for(source=0;source<10;source++)
		{
	MPI_Recv(&sum,1,MPI_INT,source,tag,MPI_COMM_WORLD,&status);
			printf(" %d %d \n",sum,source);
		}

	}


	MPI_Finalize();

}
