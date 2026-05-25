# cmakelist.txt : MSVC properties

if ( MSVC )

    set_target_properties (${ProjectName}
        PROPERTIES
        MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:debug>:Debug>DLL"
    )

    # remove any warning setup flags
    string(REGEX REPLACE "/W[0-4]" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
    string(REGEX REPLACE "-W[0-4]" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
   
    # set Winv32 defines
    target_compile_definitions(${ProjectName} PUBLIC
        _UNICODE
        UNICODE
         WIN_VER=0x600
        _WIN32_WINNT=0x600
        _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS
    )
    
    target_compile_options(${ProjectName} PUBLIC 
        -W4             # Set warning level
        -permissive-    # Specify standards conformance mode
        -wd4201         # Suppress: nonstandard extension used: nameless struct/union
        -wd4514         # Suppress: unreferenced inline function has been removed
        -wd5045         # Suppress: Compiler will insert Spectre mitigation for memory load if /Qspectre switch specified
    )

endif()
