#include "Alloc.h"
#include "ArithmeticMeanFunction.h"
#include "DataProcessor.h"
#include "MPIDataProcessor.h"
#include "MagicFuntion.h"
#include "SequentialDataProcessor.h"
#include "SimpleInitialDataGenerator.h"
#include "mpi.h"
#include <cstdio>
#include <iomanip>
#include <iostream>
#include <limits>
#include <math.h>
#include <numeric>
#include <vector>
// mpic++ -o app *.cpp -lm -lstdc++
// mpirun -np 1 ./app

using namespace std;

int calcDataPortion(int margin) {
  int dataPortion = margin * 2 + 1;
  dataPortion *= dataPortion;
  cout << dataPortion << endl;
  return dataPortion;
}

void showTable(double **table, int dataSize) {
  cout << "----------------------------------" << endl;
  for (int i = 0; i < dataSize; i++) {
    cout << setw(3) << i << " -> ";
    for (int j = 0; j < dataSize; j++)
      cout << " " << showpoint << setw(4) << setprecision(3) << table[i][j];
    cout << endl;
  }
}

int main(int argc, char *argv[]) {

  MPI_Init(&argc, &argv);
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  const int MARGIN = 1;
  const int DATA_SIZE = 5;
  const int REPETITIONS = 1;

  int dataPortion = calcDataPortion(MARGIN);
  MagicFuntion *mf = new ArithmeticMeanFunction(dataPortion);
  DataProcessor *dp = new MPIDataProcessor();
  dp->setMagicFunction(mf);

  if (rank == 0) {
    double **initialData = tableAlloc(DATA_SIZE);
    InitialDataGenerator *generator = new SimpleInitialDataGenerator(1, 10);
    generator->fillWithData(initialData, DATA_SIZE, MARGIN);
    showTable(initialData, DATA_SIZE);

    dp->setInitialData(initialData, DATA_SIZE);
  }

  dp->execute(REPETITIONS);

  if (rank == 0) {
    double **result = dp->getResult();
    showTable(result, DATA_SIZE);
  }

  MPI_Finalize();

  return 0;
}
// #define n 100

// int a[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

// // Temporary array initialfor slave process
// int a2[1000];

// int main(int argc, char *argv[]) {

//   int rank, size, elements_per_process, n_elements_recieved;
//   // np -> no. of processes
//   // pid -> process id

//   MPI_Status status;

//   // Creation of parallel processes
//   MPI_Init(&argc, &argv);

//   // find out process ID,
//   // and how many processes were started
//   MPI_Comm_rank(MPI_COMM_WORLD, &rank);
//   MPI_Comm_size(MPI_COMM_WORLD, &size);

//   // master process
//   if (rank == 0) {
//     int index, i;
//     elements_per_process = n / size;

//     // check if more than 1 processes are run
//     if (size > 1) {
//       // distributes the portion of array
//       // to child processes to calculate
//       // their partial sums
//       for (i = 1; i < size - 1; i++) {
//         index = i * elements_per_process;
//         std::cout << elements_per_process << std::endl;

//         MPI_Send(&elements_per_process, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
//         MPI_Send(&a[index], elements_per_process, MPI_INT, i, 0,
//                  MPI_COMM_WORLD);
//       }

//       // last process adds remaining elements
//       index = i * elements_per_process;
//       int elements_left = n - index;
//       std::cout << elements_per_process << std::endl;
//       MPI_Send(&elements_left, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
//       MPI_Send(&a[index], elements_left, MPI_INT, i, 0, MPI_COMM_WORLD);
//     }

//     // master process add its own sub array
//     int sum = 0;
//     for (i = 0; i < elements_per_process; i++)
//       sum += a[i];

//     // collects partial sums from other processes
//     int tmp;
//     for (i = 1; i < size; i++) {
//       MPI_Recv(&tmp, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD,
//       &status);

//       std::cout << tmp << std::endl;
//       int sender = status.MPI_SOURCE;

//       sum += tmp;
//     }

//     // prints the final sum of array
//     printf("Sum of array is : %d\n", sum);
//   }
//   // slave processes
//   else {
//     MPI_Recv(&n_elements_recieved, 1, MPI_INT, 0, 0, MPI_COMM_WORLD,
//     &status);

//     // stores the received array segment
//     // in local array a2
//     MPI_Recv(&a2, n_elements_recieved, MPI_INT, 0, 0, MPI_COMM_WORLD,
//     &status);

//     // calculates its partial sum
//     int partial_sum = 0;
//     for (int i = 0; i < n_elements_recieved; i++)
//       partial_sum += a2[i];

//     // sends the partial sum to the root process
//     MPI_Send(&partial_sum, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
//   }

//   // cleans up all MPI state before exit of process
//   MPI_Finalize();

//   return 0;
// }

// int main(int argc, char **argv) {

//   if (MPI_Init(&argc, &argv) != MPI_SUCCESS) {
//     perror("Error initializing MPI");
//     exit(1);
//   }
//   const int MARGIN = 1;
//   const int DATA_SIZE = 5;
//   const int REPETITIONS = 1;

//   int dataPortion = calcDataPortion(MARGIN);
//   MagicFuntion *mf = new ArithmeticMeanFunction(dataPortion);
//   DataProcessor *dp = new MPIDataProcessor();
//   dp->setMagicFunction(mf);

//   int size, rank;
//   MPI_Comm_size(MPI_COMM_WORLD, &size); // Get number of processes
//   MPI_Comm_rank(MPI_COMM_WORLD, &rank); // Get own ID

//   int *matrix;

//   if (rank == 0) {
//     matrix = createMatrix(size, size); // Master process creates matrix
//     printf("Initial matrix:\n");
//     printArray(matrix, size * size);
//   }

//   int *procRow =
//       (int *)malloc(sizeof(int) * size); // received row will contain p
//       integers
//   if (procRow == NULL) {
//     perror("Error in malloc 3");
//     exit(1);
//   }

//   if (MPI_Scatter(matrix, size,
//                   MPI_INT, // send one row, which contains p integers
//                   procRow, size,
//                   MPI_INT, // receive one row, which contains p integers
//                   0, MPI_COMM_WORLD) != MPI_SUCCESS) {

//     perror("Scatter error");
//     exit(1);
//   }

//   printf("Process %d received elements: ", rank);
//   printArray(procRow, size);

//   MPI_Finalize();

//   return 0;
// }
