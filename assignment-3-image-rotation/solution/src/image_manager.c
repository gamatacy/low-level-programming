#include "../include/image_manager.h"
#include <stdlib.h>

struct image create_image(size_t width, size_t height){
    return (struct image)
    {
        .width = width,
        .height = height,
        .data = malloc(width * height * sizeof(struct pixel))
    };
}

void destroy_image(struct image const* img){
    free(img->data);
}
