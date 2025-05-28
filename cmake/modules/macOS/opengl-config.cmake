if(NOT TARGET OpenGL::GL)
    add_library(OpenGL::GL INTERFACE IMPORTED)
    set_target_properties(OpenGL::GL PROPERTIES
        INTERFACE_LINK_LIBRARIES "-F${OSXCROSS_FRAMEWORKS_DIR} -framework OpenGL -framework Cocoa -framework IOKit -framework QuartzCore"
        INTERFACE_INCLUDE_DIRECTORIES "${OSXCROSS_FRAMEWORKS_DIR}/OpenGL.framework/Headers;${OSXCROSS_FRAMEWORKS_DIR}/CoreFoundation.framework/Headers;${OSXCROSS_FRAMEWORKS_DIR}/Cocoa.framework/Headers;${OSXCROSS_FRAMEWORKS_DIR}/IOKit.framework/Headers"
    )
    message(STATUS "Configured OpenGL::GL with macOS frameworks for osxcross")
endif()