#include "image.h"
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>

#ifndef IMAGE_TRANSFORMER_BMP_H
#define IMAGE_TRANSFORMER_BMP_H


#include <stdlib.h>

#define HEADERS_COUNT_TO_READ_AND_WRITE 1
#define MULTIPLICITY_OF_WIDTH 4
#define PIXEL_SIZE sizeof(struct pixel)
#define HEADER_SIZE sizeof(struct bmp_header)
#define BMP_TYPE 0x4D42
#define BMP_RESERVED 0
#define BMP_SIZE 40
#define BMP_PLANES 1
#define BMP_BIT_COUNT 24
#define BMP_COMPRESSION 0
#define BMP_X_PELS_PER_METER 1
#define BMP_Y_PELS_PER_METER 1
#define BMP_CLR_USED 0
#define BMP_CLR_IMPORTANT 0

#define INVALID_HEADER_ERROR "Can not read image header!"
#define INVALID_SIGNATURE_ERROR "Read invalid signature from bmp file!"
#define INVALID_BITS_ERROR "Read invalid bits from bmp file!"
#define BMP_READ_ERROR "Can not read bmp file!"
#define WRITE_ERROR_MESSAGE "Не могу записать файл!"

/*  deserializer   */
enum read_bmp_status {
    READ_OK = 0,
    READ_INVALID_SIGNATURE,
    READ_INVALID_BITS,
    READ_INVALID_HEADER,
    READ_ERROR
    /* коды других ошибок  */
};

enum read_bmp_status from_bmp(FILE *in, struct image *img);

/*  serializer   */
enum write_bmp_status {
    WRITE_OK = 0,
    WRITE_ERROR
    /* коды других ошибок  */
};

enum write_bmp_status to_bmp(FILE *out, struct image const *img);


struct bmp_header {
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
} __attribute__((packed));

#endif //IMAGE_TRANSFORMER_BMP_H
