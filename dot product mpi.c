#include<stdio.h>
#include<string.h>
#include "mpi.h"
AKIAXPS6J6LMEASJEFWG
W5Wp6Z4CPsp5547BSEyh++IpOLHck/WrXGRCUNju

int main( int argc, char *argv[])
{
    int my_rank,p,source,dest,tag=0;
    int X[10]={1,2,3,4,5,6,7,8,9,10};
    int Y[10]={1,2,3,4,5,6,7,8,9,10};
    int size = 10;
    MPI_Status status;
    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD,&p);

    if(my_rank!=0){
        int temp_sum = 0;
        int x = size/(p-1);
        int i;

        for(i= my_rank*x-x;i<my_rank*x && i<size;i++)
            temp_sum+=(X[i]*Y[i]);
        if(my_rank==p-1)
            while(i<size){
                temp_sum+=(X[i]*Y[i]);
                i++;
        }
        dest = 0;
        MPI_Send(&temp_sum,1,MPI_INT,dest,tag,MPI_COMM_WORLD);
    }

     if(my_rank == 0){
        int sum=0;
        for(source = 1;source < p;source++){
            int temp=0;
            MPI_Recv(&temp,1,MPI_INT,source,tag,MPI_COMM_WORLD,&status);
            //printf("%s\n",message);
            sum += temp;
        }
        printf("Dot Product = %d\n",sum);

    }

    MPI_Finalize();
    return 0;
}
