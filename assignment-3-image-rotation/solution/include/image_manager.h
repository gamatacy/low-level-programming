#ifndef image_manager
#define image_manager
#include <stdint.h>
#include <stdlib.h>

struct pixel { uint8_t b, g, r; };

struct image {
  uint64_t width, height;
  struct pixel* data;
};

struct image create_image(size_t width, size_t height);
void destroy_image(struct image const* img);
#endif

