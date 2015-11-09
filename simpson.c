#include<stdio.h>
#include<mpi.h>
#include<string.h>
double function(double x)
{
    return x*x;
}
main(int argc, char **argv)
{
    int ierr,num_procs ,my_id,tag=0,source,dest;
    char message[500];
    MPI_Status status;
    MPI_Init(&argc ,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&my_id);
    MPI_Comm_size(MPI_COMM_WORLD,&num_procs);
    double a=0.0,b=10.0;
    if(my_id!=0)
    {
        double result=(b-a)/(num_procs);
        double height=a+my_id*result;
        double area=function(a+my_id*result);
        if(my_id&1)
        area*=4;
        else
        area*=2;
        //printf("%lf,%lf,%dn",area,result,my_id);
        sprintf(message,"%lf",area);
        dest=0;
        MPI_Send(message,strlen(message),MPI_CHAR,dest,tag,MPI_COMM_WORLD);
    }
    else
    {
        double integral=(function(a)+function(b));
        for(source=1;source<num_procs;source++)
        {
            MPI_Recv(message,500,MPI_CHAR,source,tag,MPI_COMM_WORLD,&status);
            double result;
            sscanf(message,"%lf",&result);
            integral+=result;
        }
        printf("%lfn",integral*(b-a)/(num_procs*3));
    }
    MPI_Finalize();
}
