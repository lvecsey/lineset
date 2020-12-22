
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "mini_gxkit.h"

int main(int argc, char *argv[]) {

  image_t img;

  ssize_t bytes_written;

  int out_fd;

  size_t img_sz;
  long int num_pixels;

  long int nend;

  long int n;

  point_t pt;

  double weights[2];

  pixel_t white;

  long int xpos, ypos;
  
  img.xres = 960;
  img.yres = 960;

  num_pixels = img.xres * img.yres;
  img_sz = num_pixels * sizeof(pixel_t);

  img.rgb = malloc(img_sz);
  if (img.rgb == NULL) {
    perror("malloc");
    return -1;
  }

  weights[0] = 0.23;
  weights[1] = 0.45;

  white = (pixel_t) { .r = 65535, .g = 65535, .b = 65535 };

  nend = 225000;
  
  for (n = 0; n < nend; n++) {

    pt.x = -1.0 + (2.0 * n) / nend;

    pt.y = weights[0] + weights[1] * pt.x;


    xpos = pt.x * (img.xres >> 1); xpos += img.xres >> 1;
    ypos = pt.y * (img.yres >> 1); ypos += img.yres >> 1;    

    img.rgb[ypos * img.xres + xpos] = white;
    
  }
    
  out_fd = 1;
  bytes_written = write(out_fd, img.rgb, img_sz);
  if (bytes_written != img_sz) {
    perror("write");
    return -1;
  }
  
  free(img.rgb);
  
  return 0;

}
