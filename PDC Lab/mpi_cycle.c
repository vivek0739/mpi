#include<stdio.h>
#include<string.h>
#include "mpi.h"

int main( int argc, char *argv[])
{
	int my_rank,p,source,dest,tag=0;
	char message[100];
	MPI_Status status;
	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	MPI_Comm_size(MPI_COMM_WORLD,&p);
	
	sprintf( message,"from %d process !" ,my_rank);

	dest = (my_rank+1)%p;
	MPI_Send(message,strlen(message)+1,MPI_CHAR,dest,tag,MPI_COMM_WORLD);
	
	source = my_rank-1;
 	if(my_rank < 0)
		source = p - my_rank;	
	MPI_Recv(message,100,MPI_CHAR,source,tag,MPI_COMM_WORLD,&status);
	
	printf("%s to %d\n",message,my_rank);
		
	
	MPI_Finalize();
	return 0;
}
