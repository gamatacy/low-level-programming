#include "../include/rotate.h"
#include "../include/bmp.h"
#include "../include/image_manager.h"


struct image rotate(const struct image* img) {
    struct image img_copy = create_image(img->height,img->width);

    for (size_t i = 0; i < img->height; i++) {
        for (size_t j = 0; j < img->width; j++) {
          img_copy.data[ img->height*j + img->height-i-1 ] = img->data[ img->width*i + j ];
        }
    }

    return img_copy;
}
