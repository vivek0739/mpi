#include<stdio.h>
#include<string.h>
#include "mpi.h"
main(int argc,char *argv[])
{
    int my_rank,p,source,dest,tag=0;
    char message[100];
    MPI_Status status;
    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
    MPI_Comm_size(MPI_COMM_WORLD,&p);
    int sum=0;
    if(my_rank !=0 )
    {
        sprintf(message,"%d",my_rank);
        MPI_Send(message,strlen(message)+1,MPI_CHAR,dest,tag,MPI_COMM_WORLD);
    }
    else
    {
        for(source = 1 ;source<p;source++)
        {
            MPI_Recv(message,100,MPI_CHAR,source,tag,MPI_COMM_WORLD,&status);
            int temp=0,p=0;
            sscanf(message,"%d",&temp);
            sum+=temp;
        }
        printf("%d\n",sum);
    }
    MPI_Finalize();
}
