cmake_minimum_required(VERSION 3.16)
project(ITED_BASELINE)

find_package(Geant4 REQUIRED ui_all vis_all)
include(${Geant4_USE_FILE})

include_directories(${PROJECT_SOURCE_DIR}/include)

file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

add_executable(ited ${sources} ${headers})
target_link_libraries(ited ${Geant4_LIBRARIES})
