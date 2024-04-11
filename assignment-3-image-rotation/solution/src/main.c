#include "main.h"

static void printerr(const char *error) {
    fprintf(stderr, "%s", error);
}

static int8_t check_args_count(int32_t argc) {
    if (argc != ARGUMENTS_COUNT) {
        printerr(WRONG_ARGUMENTS_COUNT_ERROR);
        return 0;
    }
    return 1;
}

static int8_t check_open_file_status(enum open_status status) {
    if (status == OPEN_ERROR) {
        printerr(CAN_NOT_OPEN_FILE_ERROR);
        return 0;
    }
    return 1;
}

static uint8_t check_read_bmp_status(const enum read_bmp_status status) {
    switch (status) {
        case READ_INVALID_HEADER:
            printerr(INVALID_HEADER_ERROR);
            return 0;
        case READ_ERROR:
            printerr(BMP_READ_ERROR);
            return 0;
        case READ_OK:
        case READ_INVALID_SIGNATURE:
            printerr(INVALID_SIGNATURE_ERROR);
        case READ_INVALID_BITS:
            printerr(INVALID_BITS_ERROR);
            break;
    }
    return 1;
}

static uint8_t check_write_bmp_status(const enum write_bmp_status status) {
    if (status == WRITE_ERROR) {
        printerr(WRITE_ERROR_MESSAGE);
        return 0;
    }
    return 1;
}

static uint8_t check_parse_status(enum parse_status status) {
    switch (status) {
        case PARSE_OK:
            break;
        case PARSE_WRONG_ANGLE:
            printerr(WRONG_ANGLE_ERROR);
            return 0;
        case PARSE_ERROR:
            printerr(WRONG_ARGUMENTS_COUNT_ERROR);
            return 0;
    }
    return 1;
}

int32_t main(int32_t argc, char *argv[]) {
    (void) argc;
    (void) argv; // supress 'unused parameters' warning
    //  Проверяем число аргументов
    if (!check_args_count(argc)) return 1;

    //  Парсим угол
    uint16_t angle;
    enum parse_status parseStatus = parse(argv[ANGLE_INDEX], &angle);
    check_parse_status(parseStatus);

    //  Открываем файл только для чтения
    FILE *source_file;
    enum open_status source_file_status = open_file(argv[SOURCE_FILE_PATH_INDEX], READ_ONLY, &source_file);

    // Проверка кода открытия файла
    if (!check_open_file_status(source_file_status)) return 1;

    //  Считывание битмапа
    struct image *img = malloc(sizeof(struct image));
    enum read_bmp_status read_bmp_status = from_bmp(source_file, img);
    if (!check_read_bmp_status(read_bmp_status)) {
        free(img->data);
        free(img);
        return 1;
    }

    if (fclose(source_file) == EOF) {
        printerr(CAN_NOT_CLOSE_FILE_ERROR);
    }

    //  Выполняем поворот картинки
    rotate(img, angle);

    //  Открываем файл для записи
    FILE *target_file;
    enum open_status target_file_status = open_file(argv[TARGET_FILE_PATH_INDEX], READ_AND_WRITE, &target_file);

    // Проверка кода открытия файла
    if (!check_open_file_status(target_file_status)) {
        free(img->data);
        free(img);
        return 1;
    }

    //  Проверка кода записи в файл
    enum write_bmp_status write_bmp_status = to_bmp(target_file, img);
    if (!check_write_bmp_status(write_bmp_status)) {
        free(img->data);
        free(img);
        return 1;
    }

    if (fclose(target_file) == EOF) {
        printerr(CAN_NOT_CLOSE_FILE_ERROR);
    }

    free(img->data);
    free(img);
    return 0;
}
