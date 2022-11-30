#include "../include/file_manager.h"
#include "../include/image_manager.h"
#include "../include/rotate.h"
#include <stdio.h>

int main(int argc, char** argv ) {
	if (argc != 3) {
		fprintf(stderr, "Wrong amount of arguments");
		return 1;
	}
	
	FILE* in = NULL;
	FILE* out = NULL;
	
	if (!open_file(&in, argv[1], "rb")) {
		fprintf(stderr, "Cannot open the file to read");
		return 1;
	}
	
	if (!open_file(&out, argv[2], "wb")) {
		fprintf(stderr, "Cannot open the file to write");
		close_file(in);
		return 1;
	}
	
	printf("Files opened\n");
	
	struct image img = {0};
	if (from_bmp(in, &img) != READ_OK) {
		fprintf(stderr, "Cannot read the file");
		close_file(in);
		close_file(out);
		return 1;
	}
	
	printf("BMP readed\n");
	struct image rotated_image = rotate(&img);

	if (rotated_image.width != 0 && to_bmp(out, &rotated_image) != WRITE_OK) {
		fprintf(stderr, "Cannot write in the file");
		destroy_image(&rotated_image);
		destroy_image(&img);
		close_file(in);
		close_file(out);
		return 1;
	}
	
	printf("Rotation's done");
	
	if(!close_file(in) || !close_file(out)){
		fprintf(stderr, "Cannot close file");
		destroy_image(&img);
		destroy_image(&rotated_image);
		return 1;
	}
	
    destroy_image(&img);
	destroy_image(&rotated_image);	
    return 0;
}

