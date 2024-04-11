#include "bmp.h"

static struct bmp_header *create_header(struct image const *img, const uint64_t padding) {
    struct bmp_header *header = malloc(HEADER_SIZE);
    header->bfType = BMP_TYPE;
    header->bfileSize = HEADER_SIZE + PIXEL_SIZE * (img->width + padding) * img->height;
    header->bfReserved = BMP_RESERVED;
    header->bOffBits = HEADER_SIZE;
    header->biSize = BMP_SIZE;
    header->biWidth = img->width;
    header->biHeight = img->height;
    header->biPlanes = BMP_PLANES;
    header->biBitCount = BMP_BIT_COUNT;
    header->biCompression = BMP_COMPRESSION;
    header->biSizeImage = (img->width + padding) * img->height;
    header->biXPelsPerMeter = BMP_X_PELS_PER_METER;
    header->biYPelsPerMeter = BMP_Y_PELS_PER_METER;
    header->biClrUsed = BMP_CLR_USED;
    header->biClrImportant = BMP_CLR_IMPORTANT;

    return header;
}

static uint8_t calc_padding(uint64_t width) {
    return MULTIPLICITY_OF_WIDTH - width * PIXEL_SIZE % MULTIPLICITY_OF_WIDTH;
}

enum write_bmp_status to_bmp(FILE *out, struct image const *img) {
    uint8_t padding = calc_padding(img->width);
    struct bmp_header *header = create_header(img, padding);
    if (fwrite(header, HEADER_SIZE, HEADERS_COUNT_TO_READ_AND_WRITE, out) != HEADERS_COUNT_TO_READ_AND_WRITE) {
        free(header);
        return WRITE_ERROR;
    }

    for (uint32_t i = 0; i < img->height; i++) {
        if (fwrite(img->data + i * img->width, PIXEL_SIZE, img->width, out) != img->width) {
            free(img->data);
            free(header);
            return WRITE_ERROR;
        }
        fseek(out, padding, SEEK_CUR);
    }
    free(header);
    return WRITE_OK;
}

static struct pixel *read_pixels(FILE *in, uint64_t const height, uint64_t const width) {
    struct pixel *pixels = malloc(PIXEL_SIZE * height * width);
    uint8_t padding = calc_padding(width);

    for (uint32_t i = 0; i < height; i++) {
        if (fread(pixels + i * width, PIXEL_SIZE, width, in) != width) {
            free(pixels);
            return 0;
        }
        fseek(in, padding, SEEK_CUR);
    }
    return pixels;
}

enum read_bmp_status from_bmp(FILE *in, struct image *img) {
    struct bmp_header *header = malloc(HEADER_SIZE);
    if (fread(header, HEADER_SIZE, HEADERS_COUNT_TO_READ_AND_WRITE, in) != HEADERS_COUNT_TO_READ_AND_WRITE) {
        free(header);
        return READ_INVALID_HEADER;
    }

    img->height = header->biHeight;
    img->width = header->biWidth;
    img->data = read_pixels(in, img->height, img->width);
    free(header);
    if (!img->data) return READ_ERROR;
    return READ_OK;
}
