
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <sys/mman.h>
#include <errno.h>
#include <string.h>

#include "mini_gxkit.h"

#include "mse.h"

int main(int argc, char *argv[]) {

  point_t pt;

  double weights[2];

  char *trainset_fn;

  int fd;
  struct stat buf;
  void *m;

  int retval;

  double *samples;

  double *observations;

  double *responses;

  long int num_samples;

  long int sampleno;

  double lr;

  uint64_t rnds[4];
  
  int rnd_fd;

  long int max_trainings;

  long int trainno;

  long int num_rnds;

  ssize_t bytes_read;

  double nextw[2];

  long int counter;

  double MSE;

  double *artificial_responses;
  
  rnd_fd = open("/dev/urandom", O_RDONLY);
  if (rnd_fd == -1) {
    perror("open");
    return -1;
  }
  
  trainset_fn = argc>1 ? argv[1] : NULL;

  fd = open(trainset_fn, O_RDWR);
  retval = fstat(fd, &buf);
  m = mmap(NULL, buf.st_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
  if (m == MAP_FAILED) {
    perror("mmap");
    return -1;
  }

  num_samples = buf.st_size / (sizeof(double) * 2);
  
  samples = (double*) m;

  observations = samples;

  responses = samples + num_samples;
  
  weights[0] = 0.50;
  weights[1] = 0.70;

  printf("Starting weights %g %g\n", weights[0], weights[1]);
  
  lr = 0.23;
  
  printf("Learning rate lr %g\n", lr);
  
  num_rnds = sizeof(rnds) / sizeof(uint64_t);
  
  max_trainings = 45;

  trainno = 0;

  artificial_responses = malloc(sizeof(double) * num_samples);
  if (artificial_responses == NULL) {
      perror("malloc");
      return -1;
  }
  
  do {
  
    nextw[0] = 0.0;
    nextw[1] = 0.0;

    for (counter = 0; counter < num_samples; counter++) {

      bytes_read = read(rnd_fd, rnds, sizeof(uint64_t) * num_rnds);
      if (bytes_read != sizeof(uint64_t) * num_rnds) {
	perror("read");
	return -1;
      }

      sampleno = (rnds[0] * ((double) num_samples)) / 18446744073709551615.0;

      pt = (point_t) { .x = observations[sampleno], .y = responses[sampleno] };

      nextw[0] += (2 * (weights[0] + weights[1] * pt.x - pt.y));

      nextw[1] += (2 * pt.x * (weights[0] + weights[1] * pt.x - pt.y));

    }

    nextw[0] /= num_samples;
    nextw[1] /= num_samples;
    
    printf("Current nextw %g %g\n", nextw[0], nextw[1]);

    weights[0] -= lr * nextw[0];
    weights[1] -= lr * nextw[1];
    
    printf("Current weights %g %g\n", weights[0], weights[1]);    

    {

      long int artificialno;

      point_t art_pt;
      
      for (artificialno = 0; artificialno < num_samples; artificialno++) {

	art_pt.x = observations[artificialno];

	art_pt.y = weights[0] + weights[1] * art_pt.x;
	
	artificial_responses[artificialno] = art_pt.y;
	
      }

    }
    
    MSE = mse(responses, artificial_responses, num_samples);

    printf("[%ld] MSE %g\n", trainno, MSE);

    trainno++;
    
  } while (trainno < max_trainings && MSE >= 1.0e-6);

  free(artificial_responses);
  
  retval = munmap(m, buf.st_size);
  if (retval == -1) {
    perror("munmap");
    return -1;
  }

  retval = close(fd);
  if (retval == -1) {
    perror("close");
    return -1;
  }
  
  retval = close(rnd_fd);
  if (retval == -1) {
    perror("close");
    return -1;
  } 

  return 0;

}
