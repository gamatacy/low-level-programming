#include "../include/bmp.h"
#include "../include/image_manager.h"
#include <stdlib.h>

#define FORMAT_TYPE 0x4d42
#define INFO_HEADER_SIZE 40
#define PLANES 1
#define BIT_COUNT 24


static inline uint8_t get_padding(size_t width){
    uint8_t width_modulo = ( width * sizeof(struct pixel) ) % 4 ;
    if (width_modulo){
        return 4 - width_modulo;
    }
    return 0;
}

struct bmp_header create_header(struct image const* img) {
	uint32_t image_size = (uint32_t)((sizeof(struct pixel) * img->width + get_padding(img->width)) * img->height);
	return (struct bmp_header) 
    {
		.bfType = FORMAT_TYPE,
        .bfileSize = sizeof(struct bmp_header) + image_size,
        .bfReserved = 0,
        .bOffBits = sizeof(struct bmp_header),
        .biSize = INFO_HEADER_SIZE,
        .biWidth = (uint32_t)img->width,
        .biHeight = (uint32_t)img->height,
        .biPlanes = PLANES,
        .biBitCount = BIT_COUNT,
        .biCompression = 0,
        .biSizeImage = image_size,
        .biXPelsPerMeter = 0,
        .biYPelsPerMeter = 0,
        .biClrUsed = 0,
        .biClrImportant = 0
	};
}

enum read_status read_header(FILE* in, struct bmp_header* header){
    if (fread(header, sizeof(struct bmp_header), 1, in)) {
		return READ_OK;
	} else {
		return READ_INVALID_HEADER;
	}
}

enum write_status write_header(FILE* out, struct bmp_header const* header) {
	if (fwrite(header, sizeof(struct bmp_header), 1, out) == 1) {
		return WRITE_OK;
	} else {
		return WRITE_ERROR;
	}
}

enum read_status from_bmp(FILE* in, struct image* img){
    struct bmp_header header = {0};
    if (read_header(in, &header) == WRITE_OK) {
       
        img->width = header.biWidth;
		img->height = header.biHeight;
	
        struct image img_new = create_image(img->width, img->height);
        if(img_new.data == NULL) return READ_INVALID_BITS;
        for (size_t i = 0; i < img_new.height; i++) {
            if (fread(img_new.data + (i * img_new.width), sizeof(struct pixel), img_new.width, in) != img_new.width) {
                destroy_image(&img_new);
                return READ_INVALID_SIGNATURE;
            }
            if (fseek(in, get_padding(img_new.width), SEEK_CUR) != 0) {
                destroy_image(&img_new);
                return READ_INVALID_SIGNATURE;
            }
	    }
        img->data = img_new.data;
        return READ_OK;
	} else {
		return READ_INVALID_HEADER;
	}
}

enum write_status to_bmp(FILE* out, struct image const* img){
    struct bmp_header header = create_header(img);
	if (write_header(out, &header) == WRITE_OK) {

		for (size_t i = 0; i < img->height; i++) {
		if (fwrite(img->data + i * img->width, sizeof(struct pixel), img->width, out) != img->width){
			return WRITE_ERROR;
		}
		if (fseek(out, get_padding(img->width), SEEK_CUR) != 0) {
			return WRITE_ERROR;
		}

	}

	return WRITE_OK;
	}
	return WRITE_ERROR;
}
