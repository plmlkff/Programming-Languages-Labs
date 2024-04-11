#include "angle_parser.h"

#define ANGLES_COUNT 7
#define BASE 10

static const int32_t angles[] = {-270, -180, -90, 0, 90, 180, 270};

enum parse_status parse(const char *angle_string, uint16_t *const angle) {
    char *err_p;
    *angle = strtol(angle_string, &err_p, BASE);
    if (*err_p != '\0') {
        return PARSE_ERROR;
    }
    for (uint8_t i = 0; i < ANGLES_COUNT; i++) {
        if (angles[i] == *angle) return PARSE_OK;
    }
    return PARSE_WRONG_ANGLE;
}

