#include "./image_manager.h"
#include <stdint.h>
#include <stdio.h>

#pragma pack(push, 1)
struct bmp_header 
{
        uint16_t bfType;
        uint32_t bfileSize;
        uint32_t bfReserved;
        uint32_t bOffBits;
        uint32_t biSize;
        uint32_t biWidth;
        uint32_t biHeight;
        uint16_t biPlanes;
        uint16_t biBitCount;
        uint32_t biCompression;
        uint32_t biSizeImage;
        uint32_t biXPelsPerMeter;
        uint32_t biYPelsPerMeter;
        uint32_t biClrUsed;
        uint32_t biClrImportant;
};
#pragma pack(pop)

enum read_status {
  READ_OK = 0,
  READ_INVALID_SIGNATURE,
  READ_INVALID_BITS,
  READ_INVALID_HEADER
  };

enum read_status from_bmp(FILE* in, struct image* img);

enum  write_status  {
  WRITE_OK = 0,
  WRITE_ERROR
};

enum write_status to_bmp(FILE* out, struct image const* img);

static inline uint8_t get_padding(size_t width);
struct bmp_header create_header(struct image const* img);
enum read_status read_header(FILE* in, struct bmp_header* header);
enum write_status write_header(FILE* out, struct bmp_header const* header);

