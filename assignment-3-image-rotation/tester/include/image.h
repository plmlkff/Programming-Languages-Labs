#pragma once

#include <inttypes.h>
#include <stddef.h>
#include <stdlib.h>

#include "dimensions.h"

struct pixel {
  uint8_t components[3];
};

struct image {
  struct dimensions size;
  struct pixel* data;
};

struct image image_create( struct dimensions size );
void image_destroy( struct image* image );
