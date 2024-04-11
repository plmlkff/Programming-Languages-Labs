#ifndef IMAGE_TRANSFORMER_ANGLE_PARSER_H
#define IMAGE_TRANSFORMER_ANGLE_PARSER_H
#define WRONG_ANGLE_ERROR "Entered wrong angle! Allowed values: [-270, -180, -90, 0, 90, 180, 270]"
#define ANGLE_ERROR "Can not parse angle!"

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>


enum parse_status {
    PARSE_OK,
    PARSE_WRONG_ANGLE,
    PARSE_ERROR
};

enum parse_status parse(const char *angle_string, uint16_t *angle);

#endif //IMAGE_TRANSFORMER_ANGLE_PARSER_H
