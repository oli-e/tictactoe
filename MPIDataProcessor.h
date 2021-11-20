/*
 * MPIDataProcessor.h
 *
 *  Created on: 13 list 2021
 *      Author: olena
 */

#ifndef MPIDATAPROCESSOR_H_
#define MPIDATAPROCESSOR_H_

#include "DataProcessor.h"

class MPIDataProcessor : public DataProcessor {
private:
  void createDataPortion(int row, int col, double *buffer);

protected:
  void singleExecution();
  void collectData() {}
  void shareData();

public:
  MPIDataProcessor(int argc, char *argv[]);
  MPIDataProcessor(){};
  ~MPIDataProcessor(){};
  double **getResult() { return data; }
};

#endif /* MPIDATAPROCESSOR_H_ */