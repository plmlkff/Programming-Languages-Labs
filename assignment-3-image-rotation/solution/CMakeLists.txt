file(GLOB_RECURSE sources CONFIGURE_DEPENDS
    src/*.c
    src/*.h
    include/*.h
)

add_executable(image-transformer ${sources}
        include/bmp.h
        include/image.h
        src/image.c
        src/bmp.c
        src/files_logic.c
        include/files_logic.h
        src/angle_parser.c
        include/angle_parser.h
        include/main.h)
target_include_directories(image-transformer PRIVATE src include)
