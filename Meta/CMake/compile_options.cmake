set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

string(COMPARE EQUAL ${CMAKE_CXX_COMPILER_FRONTEND_VARIANT} "MSVC" IS_MSVC_LIKE)

if(IS_MSVC_LIKE)
    add_compile_options(
        /W3
        /WX
        /we4100 # -Wunused-parameter
        /we4706 # -Wparentheses
        /we5262 # -Wimplicit-fallthrough
    )

    if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        add_compile_options(-Wmissing-designated-field-initializers -Wunused-parameter)
    endif()
else()
    add_compile_options(
        -Wall
        -Wextra
        -Werror
        -Wimplicit-fallthrough
        -Wno-array-bounds
        -fvisibility=hidden
    )
endif()

add_compile_definitions(QT_DISABLE_DEPRECATED_BEFORE=0x060800)
