#include <stdio.h>
#include "mpi.h"
float f(float x) {
    float return_val;
    return_val = x*x;
    return return_val;
}

float Trap(
          float  local_a,
          float  local_b,
          int    local_n ,
          float  h) {

    float integral;
    float x;
    int i;

    integral = (f(local_a) + f(local_b))/2.0;
    x = local_a;
    for (i = 1; i <= local_n-1; i++) {
        x = x + h;
        integral = integral + f(x);
    }
    integral = integral*h;
    return integral;
}

int main(int argc, char** argv) {
    int         my_rank, p;
    float       a = 0.0, b = 10.0, h;
    int n = 100000;
    float       local_a, local_b;
    int 	local_n;
    float       integral, total;
    int         source, dest = 0, tag = 0;
    MPI_Status  status;
    printf("hello\n");
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    printf("a %f  b %f\n",a,b );

    h = (b-a)/n;
    local_n = n/p;

    local_a = a + my_rank*local_n*h;
    local_b = local_a + local_n*h;
    printf("local_a %f  local_b %f\n",local_a,local_b );
    integral = Trap(local_a, local_b, local_n, h);
    

    if (my_rank == 0) {
        total = integral;
        for (source = 1; source < p; source++) {
            MPI_Recv(&integral, 1, MPI_FLOAT, source, tag, MPI_COMM_WORLD, &status);
            total = total + integral;
        }
    } else
        MPI_Send(&integral, 1, MPI_FLOAT, dest, tag, MPI_COMM_WORLD);

    if (my_rank == 0) {
        printf("With n = %d trapezoids, our estimate\n", n);
        printf("of the integral from %f to %f = %f\n", a, b, total);
    }

    MPI_Finalize();
    return 0;
}

