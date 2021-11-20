/*
 * MPIDataProcessor.cpp
 *
 *  Created on: 13 list 2021
 *      Author: olena
 */

#include "MPIDataProcessor.h"
#include "Alloc.h"
#include "mpi.h"
#include <iostream>

void MPIDataProcessor::shareData() {

  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (rank == 0) {
    nextData = tableAlloc(dataSize);
    for (int i = 0; i < dataSize; i++)
      for (int j = 0; j < dataSize; j++) {
        nextData[i][j] = data[i][j];
      }
  }
}

MPIDataProcessor::MPIDataProcessor(int argc, char *argv[]) {

  MPI_Init(&argc, &argv);
}

// MPIDataProcessor::~MPIDataProcessor() { MPI_Finalize(); }

void MPIDataProcessor::singleExecution() {
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  double *buffer = nullptr;
  double result{};

  if (rank == 0) {

    double *buffer = new double[dataPortionSize];
    for (int row = margin; row < dataSize - margin; row++)
      for (int col = margin; col < dataSize - margin; col++) {
        createDataPortion(row, col, buffer);

        if (size > 1) {
          for (auto k = 1; k < size; k++) {
            std::cout << "Root sends buffer to rank: " << k << std::endl;
            MPI_Send(buffer, dataPortionSize, MPI_DOUBLE, k, 0, MPI_COMM_WORLD);
            std::cout << "Root received result " << result
                      << " from rank: " << k << std::endl;
            MPI_Recv(&result, 1, MPI_DOUBLE, k, 0, MPI_COMM_WORLD,
                     MPI_STATUS_IGNORE);

            nextData[row][col] = result;
          }
        } else {
          nextData[row][col] = function->calc(buffer);
        }
      }

    std::cout << "ROOT: The end of the story: " << std::endl;

    if (size > 1) {
      for (auto k = 1; k < size; k++) {
        MPI_Send(buffer, 0, MPI_DOUBLE, k, 0, MPI_COMM_WORLD);
      }
    }
  } else {

    while (true) {

      MPI_Status status;
      MPI_Probe(0, 0, MPI_COMM_WORLD, &status);

      int n_items;
      MPI_Get_count(&status, MPI_INT, &n_items);
      std::cout << "Other probing number of messages: " << n_items << std::endl;

      if (n_items == 0)
        break;

      std::cout << "Other receives buffer; rank: " << rank << std::endl;

      double *recvBuffer = new double[dataPortionSize];
      MPI_Recv(recvBuffer, dataPortionSize, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD,
               MPI_STATUS_IGNORE);

      std::cout << "Other sends result: " << result << "; rank: " << rank
                << std::endl;

      result = function->calc(recvBuffer);
      MPI_Send(&result, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
    }

    std::cout << "OTHER: The end of the story: rank: " << rank << std::endl;
  }

  std::cout << "Barrier: rank: " << rank << std::endl;
  MPI_Barrier(MPI_COMM_WORLD);

  if (rank == 0) {
    delete[] buffer;
    double **tmp = data;
    data = nextData;
    nextData = tmp;
  }
}

void MPIDataProcessor::createDataPortion(int row, int col, double *buffer) {
  int counter = 0;
  for (int i = row - margin; i <= row + margin; i++)
    for (int j = col - margin; j <= col + margin; j++)
      buffer[counter++] = data[i][j];
}