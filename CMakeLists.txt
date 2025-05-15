cmake_minimum_required(VERSION 3.16)
project(GraphicManager LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin/Debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin/Release)

set(SFML_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib/SFML-2.6.0/lib/cmake/SFML")
message(STATUS "Looking for SFML in: ${SFML_DIR}")

find_package(SFML 2.6.0 EXACT COMPONENTS system window graphics REQUIRED)
message(STATUS "SFML found: ${SFML_VERSION}")

file(GLOB SOURCES src/*.cpp)
file(GLOB HEADERS include/*.h)
file(GLOB INTERFACE_ELEMENTS include/Graphics/InterfaceElements/*.h)
file(GLOB BUTTON_FACTORY include/Graphics/InterfaceElements/ButtonFactory/*.h)
file(GLOB TESTS tests/*.cpp tests/*.h)

add_executable(
    ${PROJECT_NAME} 
    ${SOURCES} 
    ${HEADERS} 
    ${INTERFACE_ELEMENTS}
    ${TESTS}
    ${BUTTON_FACTORY}
)

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/resources
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/resources
)

source_group("Source Files" FILES ${SOURCES})
source_group("Header Files" FILES ${HEADERS})
source_group("Graphics/InterfaceElements" FILES ${INTERFACE_ELEMENTS})
source_group("Tests Files" FILES ${TESTS})
source_group("Graphics/InterfaceElements/ButtonFactory" FILES ${BUTTON_FACTORY})

target_include_directories(${PROJECT_NAME} PRIVATE "include" "tests")

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
             add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${SFML_DLLS}
        $<TARGET_FILE_DIR:${PROJECT_NAME}>
)
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