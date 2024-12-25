cmake_minimum_required(VERSION 2.8.12)
project(cmake_tool LANGUAGES C CXX DESCRIPTION "A tool to manage cmake project")

include(CMakePrintHelpers)

##--------------------- Compile Options ------------------------------------##
# Specified the language standard
set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 11)

# Configure compile options
add_compile_options(-Wall -Wextra -pedantic)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

##--------------------- Global Macros --------------------------------------##
add_definitions(
)

##--------------------- Include directories --------------------------------##
include_directories(include)
include_directories(include/external/tiny-process-library)

##--------------------- Source files ---------------------------------------##
if(WIN32)
    set(TPL_SRCS "src/external/tiny-process-library/process.cpp"
                 "src/external/tiny-process-library/process_win.cpp")
else(WIN32)
    set(TPL_SRCS "src/external/tiny-process-library/process.cpp"
                 "src/external/tiny-process-library/process_unix.cpp")
endif(WIN32)

set(UTILS_SRCS "src/utils/FileUtils.cpp"
               "src/utils/RandomUtils.cpp"
               "src/utils/StringUtils.cpp"
               "src/utils/SystemUtils.cpp")

set(LIB_SRCS ${TPL_SRCS} ${UTILS_SRCS})

file(GLOB EXE_SRCS LIST_DIRECTORIES false RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

cmake_print_variables(LIB_SRCS EXE_SRCS)

##--------------------- Build target ---------------------------------------##
add_executable(${PROJECT_NAME}
    ${EXE_SRCS}
    ${LIB_SRCS}
)
target_link_libraries(${PROJECT_NAME}
    # utility
    pthread
)
# set RPATH for ${PROJECT_NAME}
set(${PROJECT_NAME}_RPATH "\$ORIGIN/../lib:\$ORIGIN/../../lib:\$ORIGIN/../../../lib")
set_target_properties(${PROJECT_NAME}
  PROPERTIES
    # OUTPUT_NAME "rename"
    LINK_FLAGS "-Wl,--disable-new-dtags"
    INSTALL_RPATH_USE_LINK_PATH ON
    INSTALL_RPATH "${${PROJECT_NAME}_RPATH}"
)

# add_library(utility
#     SHARED
#     ${LIB_SRCS}
# )
# target_link_libraries(utility
#     pthread
# )

# add_dependencies(${PROJECT_NAME}
#     utility
# )

##--------------------- Install target ---------------------------------------##
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR} CACHE PATH "Default install prefix" FORCE)
endif()
cmake_print_variables(CMAKE_INSTALL_PREFIX)

install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
)

# get_filename_component(PROJECT_ROOT ${CMAKE_CURRENT_BINARY_DIR} PATH)

