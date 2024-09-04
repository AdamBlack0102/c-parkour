cmake_minimum_required(VERSION 3.26)
project(final_project C)

set(CMAKE_C_STANDARD 11)

include_directories("D:\\CPL\\mingw(CLion+VSC)\\mingw(CLion+VSC)\\SDL2-2.26.0-allinone\\x86_64-w64-mingw32\\include")
link_directories("D:\\CPL\\mingw(CLion+VSC)\\mingw(CLion+VSC)\\SDL2-2.26.0-allinone\\x86_64-w64-mingw32\\lib")

add_compile_options(-fcommon)

add_subdirectory(srcs)

include_directories(include)

target_link_directories(final_project PRIVATE
        mingw32
        SDL2main
        SDL2
        SDL2_image
        SDL2_mixer
        SDL2_ttf
)

target_link_libraries(final_project PRIVATE
        SDL2
        SDL2main
        SDL2_image
        SDL2_mixer
        SDL2_ttf
)
