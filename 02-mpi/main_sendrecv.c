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
                MPI_Sendrecv(
                    array, 5, MPI_INT, 1, 42,
                    &array[5], 5, MPI_INT, 1, 42,
                    MPI_COMM_WORLD, &status
                );
            }
            break;
        case 1:
            {   
                for (i = 5; i < 10; i++)
                {
                    array[i] = i;
                }
                MPI_Sendrecv(
                    &array[5], 5, MPI_INT, 0, 42,
                    array, 5, MPI_INT, 0, 42,
                    MPI_COMM_WORLD, &status
                );
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

    char errstr[MPI_MAX_ERROR_STRING + 1];
    int errstr_l;
    MPI_Error_string(status.MPI_ERROR, errstr, &errstr_l);
    errstr[errstr_l] = '\0';
    printf("%s [%d]\n", errstr, status.MPI_ERROR);

    for (i = 0; i < 10; i++)
    {
        printf("%d ", array[i]);
    }
    printf("\n");
    printf("Elapsed time %fs\n", elapsed);

    MPI_Finalize();
    return 0;
}
