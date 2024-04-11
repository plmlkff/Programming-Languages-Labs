#include <inttypes.h>
#include <stdlib.h>

#ifndef IMAGE_TRANSFORMER_IMAGE_H
#define IMAGE_TRANSFORMER_IMAGE_H
#define ANGLE_OF_ROTATION 90
#define CIRCLE_DEGREES 360

#pragma pack(push, 1)
struct pixel {
    uint8_t r, g, b;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct image {
    uint64_t width, height;
    struct pixel *data;
};
#pragma pack(pop)

void rotate(struct image *img, uint16_t angle);

#endif //IMAGE_TRANSFORMER_IMAGE_H

// I'll have watched the movie until father comes back
// By the time you arrive, I will have finished cleaning up.
