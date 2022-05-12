cmake_minimum_required(VERSION 3.17)
project(PSP_Tutorial)

set(CMAKE_C_STANDARD 11)

include_directories(include)

add_executable(PSP_Tutorial main.c)

if (PLATFORM_PSP)

    create_pbp_file (
        TARGET PSP_Tutorial
        ICON_PATH NULL
        PREVIEW_PATH NULL
        TITLE PSP Tutorial
        BUILD_PRX
    )
endif()