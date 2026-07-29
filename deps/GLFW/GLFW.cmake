
ExternalProject_Add(
    GLFW
    URL https://github.com/glfw/glfw/releases/download/3.4/glfw-3.4.zip
    URL_HASH SHA256=b5ec004b2712fd08e8861dc271428f048775200a2df719ccf575143ba749a3e9
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    CMAKE_ARGS
        -DGLFW_BUILD_DOCS=OFF
        -DGLFW_BUILD_EXAMPLES=OFF
        -DGLFW_BUILD_TESTS=OFF
        -DCMAKE_INSTALL_PREFIX:STRING=${DESTDIR}
    BUILD_BYPRODUCTS
        <INSTALL_DIR>/lib/libglfw3$<$<CONFIG:Debug>:d>.a
    )
