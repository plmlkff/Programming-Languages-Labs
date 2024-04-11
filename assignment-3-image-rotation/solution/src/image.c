#include "image.h"

static struct image *create_image_copy(struct image *const img) {
    struct image *img_copy = malloc(sizeof(struct image));
    img_copy->data = malloc(sizeof(struct pixel) * img->height * img->width);
    img_copy->width = img->height;
    img_copy->height = img->width;
    return img_copy;
}

static void rotate_90(struct image *img) {
    struct image *img_buffer = create_image_copy(img);
    for (uint64_t i = 0; i < img_buffer->height; i++) {
        for (uint64_t j = 0; j < img_buffer->width; j++) {
            img_buffer->data[i * img_buffer->width + j] = img->data[j * img->width + img->width - i - 1];
        }
    }
    free(img->data);
    img->data = img_buffer->data;
    img->width = img_buffer->width;
    img->height = img_buffer->height;
    free(img_buffer);
}

/**
 * Поворачивает картинку на заданный угол
 * @param img указатель на картинку
 * @param angle угол
 */
void rotate(struct image *const img, uint16_t angle) {
    angle = angle < 0 ? CIRCLE_DEGREES + angle : angle;
    uint8_t rotations_count = angle / ANGLE_OF_ROTATION;
    while (rotations_count) {
        rotate_90(img);
        rotations_count--;
    }
}
