
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "square.h"

#include "mse.h"

double mse(double *responses, double *artificial_responses, long int num_values) {

  double MSE;

  long int valueno;
  
  MSE = 0.0;

  for (valueno = 0; valueno < num_values; valueno++) {
    MSE += square(responses[valueno] - artificial_responses[valueno]);
  }

  MSE /= num_values;
      
  return MSE;

}
