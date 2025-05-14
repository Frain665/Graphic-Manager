cmake_minimum_required(VERSION 3.16)
project(GraphicManager LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin/Debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin/Release)

set(SFML_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib/SFML-2.6.0/lib/cmake/SFML")
message(STATUS "Looking for SFML in: ${SFML_DIR}")

if(NOT EXISTS "${SFML_DIR}/SFMLConfig.cmake")
    message(FATAL_ERROR "SFMLConfig.cmake not found in ${SFML_DIR}")
endif()

find_package(SFML 2.6.0 EXACT COMPONENTS system window graphics REQUIRED)
message(STATUS "SFML found: ${SFML_VERSION}")

file(GLOB SOURCES src/*.cpp)
file(GLOB HEADERS include/*.h include/Graphics/*h)
file(GLOB BUTTONS include/**/*.h)
file(GLOB GRAPHICS include/Graphics/*h)
file(GLOB GRAPHICS_BUTTONS include/Graphics/Buttons/*h)
file(GLOB GRAPHICS_TEXT_FIELD include/Graphics/TextField/*h)

add_executable(
    ${PROJECT_NAME} 
    ${SOURCES} 
    ${HEADERS} 
    ${BUTTONS}
    ${GRAPHICS}
    ${GRAPHICS_BUTTONS}
    ${GRAPHICS_TEXT_FIELD}
)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/resources
     DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
)

source_group("Source Files" FILES ${SOURCES})
source_group("Header Files" FILES ${HEADERS})
source_group("Buttons" FILES ${BUTTONS})
source_group("Graphics" FILES ${GRAPHICS})
source_group("Graphics/Buttons" FILES ${GRAPHICS_BUTTONS})
source_group("Graphics/TextField" FILES ${GRAPHICS_TEXT_FIELD})

target_include_directories(${PROJECT_NAME} PRIVATE "include")

target_link_libraries(${PROJECT_NAME} PRIVATE
     sfml-system
    sfml-window
    sfml-graphics
)

if(WIN32)
    set(SFML_BIN_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib/SFML-2.6.0/bin")
    if(EXISTS "${SFML_BIN_DIR}")
        file(GLOB SFML_DLLS "${SFML_BIN_DIR}/*.dll")
        if(SFML_DLLS)
             file(COPY ${SFML_DLLS} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG})
            file(COPY ${SFML_DLLS} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE})
        else()
            message(WARNING "No DLLs found in ${SFML_BIN_DIR}")
        endif()
    else()
        message(WARNING "SFML bin directory not found: ${SFML_BIN_DIR}")
    endif()
endif()

target_compile_definitions(${PROJECT_NAME} PRIVATE 
    RESOURCES_DIR="${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources/"
)