#include "files_logic.h"

enum open_status open_file(char *const filename, enum file_open_modes open_mode, FILE **file) {
    switch (open_mode) {
        case READ_ONLY:
            *file = fopen(filename, "rd");
            break;
        case READ_AND_WRITE:
            *file = fopen(filename, "wd");
            break;
    }
    return *file ? OPEN_OK : OPEN_ERROR;
}
