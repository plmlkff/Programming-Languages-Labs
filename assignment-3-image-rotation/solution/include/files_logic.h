#ifndef IMAGE_TRANSFORMER_FILES_LOGIC_H
#define IMAGE_TRANSFORMER_FILES_LOGIC_H
#define CAN_NOT_OPEN_FILE_ERROR "Can not open file!"
#define CAN_NOT_CLOSE_FILE_ERROR "Can not close file!"

#include <stdio.h>

enum file_open_modes {
    READ_ONLY,
    READ_AND_WRITE
};

enum open_status {
    OPEN_OK,
    OPEN_ERROR
};

enum open_status open_file(char *const filename, enum file_open_modes open_mode, FILE **file);

#endif //IMAGE_TRANSFORMER_FILES_LOGIC_H
