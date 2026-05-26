if (CMAKE_VERSION VERSION_GREATER 3.12)

    set_target_properties (${TargetName} PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
    )

    set_target_properties (${TargetName}
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib/${CMAKE_BUILD_TYPE}/${ARCHITECTURE}"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib/${CMAKE_BUILD_TYPE}/${ARCHITECTURE}"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE}/${ARCHITECTURE}"
    )

endif()

# include Microsoft specific properties
include("${CMAKE_SOURCE_DIR}/config/cmake/msvc.cmake")
