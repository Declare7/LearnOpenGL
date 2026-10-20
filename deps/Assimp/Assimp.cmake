
ExternalProject_Add(
    Assimp
    URL https://github.com/assimp/assimp/archive/refs/tags/v3.1.1.zip
    # URL_HASH SHA256=b5ec004b2712fd08e8861dc271428f048775200a2df719ccf575143ba749a3e9
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    CMAKE_ARGS
        -DCMAKE_INSTALL_PREFIX:STRING=${DESTDIR}
        -DCMAKE_CXX_FLAGS=-std=c++11
    BUILD_BYPRODUCTS
        <INSTALL_DIR>/lib/libassimp$<$<CONFIG:Debug>:d>.a
    )
