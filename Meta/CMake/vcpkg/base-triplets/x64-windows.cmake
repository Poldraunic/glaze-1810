include(${CMAKE_CURRENT_LIST_DIR}/base.cmake)

set(VCPKG_CMAKE_SYSTEM_NAME)
set(VCPKG_TARGET_ARCHITECTURE x64)
set(VCPKG_CRT_LINKAGE dynamic)
set(VCPKG_LIBRARY_LINKAGE dynamic)

# OpenSSL невозможно собрать с использованием ClangCL.
if(PORT STREQUAL "openssl")
    if($ENV{CC} MATCHES "clang-cl")
        set(ENV{CC} "")
    endif()

    if($ENV{CXX} MATCHES "clang-cl")
        set(ENV{CXX} "")
    endif()
endif()
