#include <stdio.h>
#include <mpi.h>


int main(int argc, char *argv[])
{
    int array[10];
    
    int i;

    int rank, size;
    MPI_Status status;
    MPI_Request r_send, r_recv;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    double elapsed = MPI_Wtime();

    switch(rank)
    {
        case 0:
            {
                for (i = 0; i < 5; i++)
                {
                    array[i] = i;
                }
                MPI_Isend(array, 5, MPI_INT, 1, 42, MPI_COMM_WORLD, &r_send);
                MPI_Irecv(&array[5], 5, MPI_INT, 1, 42, MPI_COMM_WORLD, &r_recv);

                MPI_Wait(&r_recv, &status);
            }
            break;
        case 1:
            {   
                for (i = 5; i < 10; i++)
                {
                    array[i] = i;
                }

                MPI_Isend(&array[5], 5, MPI_INT, 0, 42, MPI_COMM_WORLD, &r_send);
                MPI_Irecv(array, 5, MPI_INT, 0, 42, MPI_COMM_WORLD, &r_recv);

                MPI_Wait(&r_recv, &status);
            }
            break;
        default:
            {
                MPI_Finalize();
                return 0;
            }
            break;
    }

    elapsed = MPI_Wtime() - elapsed;

    for (i = 0; i < 10; i++)
    {
        printf("%d ", array[i]);
    }
    printf("\n");
    printf("Elapsed time %fs\n", elapsed);

    MPI_Finalize();
    return 0;
}
