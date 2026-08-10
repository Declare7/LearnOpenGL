
ExternalProject_Add(
    GLM
    URL https://github.com/g-truc/glm/releases/download/1.0.3/glm-1.0.3.zip
    URL_HASH SHA256=1c0a0fced9b0d87c7b7bc94e40be490cff6d4c83c25db8488d8f33754e7fdeb2
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    CMAKE_ARGS
        -DCMAKE_INSTALL_PREFIX:STRING=${DESTDIR}
    BUILD_BYPRODUCTS
        <INSTALL_DIR>/lib/libglm$<$<CONFIG:Debug>:d>.a
    )
