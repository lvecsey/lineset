
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <sys/mman.h>
#include <errno.h>

#include "mini_gxkit.h"

int main(int argc, char *argv[]) {

  point_t pt;

  double weights[2];

  long int num_samples;

  long int sampleno;

  double *samples;
  
  double *observations;

  double *responses;
  
  size_t fsize;

  char *trainset_fn;

  int fd;

  void *m;

  mode_t mode;

  int retval;
  
  trainset_fn = argc>1 ? argv[1] : NULL;
  
  num_samples = 2097152;

  fsize = num_samples * sizeof(double) * 2;

  mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
  
  fd = open(trainset_fn, O_CREAT | O_RDWR, mode);
  if (fd == -1) {
    perror("open");
    return -1;
  }

  retval = ftruncate(fd, fsize);
  if (retval == -1) {
    perror("ftruncate");
    return -1;
  }

  m = mmap(NULL, fsize, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
  if (m == MAP_FAILED) {
    perror("mmap");
    return -1;
  }
  
  samples = (double*) m;

  observations = samples;

  responses = observations + num_samples;

  weights[0] = 0.52;
  weights[1] = 0.26;
  
  for (sampleno = 0; sampleno < num_samples; sampleno++) {

    pt.x = -1.0 + (2.0 * sampleno) / num_samples;

    pt.y = weights[0] + weights[1] * pt.x;

    observations[sampleno] = pt.x;

    responses[sampleno] = pt.y;
    
  }
    
  retval = munmap(m, fsize);
  if (retval == -1) {
    perror("munmap");
    return -1;
  }
  
  retval = close(fd);
  if (retval == -1) {
    perror("close");
    return -1;
  }
  
  return 0;

}
