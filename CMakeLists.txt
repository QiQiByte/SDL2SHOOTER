#标题
cmake_minimum_required(VERSION 3.10)
project(SDL2SHOOTER)

#C++标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#设置输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR})


set(TARGET ${PROJECT_NAME}-${CMAKE_SYSTEM_NAME}-${CMAKE_BUILD_TYPE})


#查找并载入cmake预设
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_mixer REQUIRED)

#添加可执行文件
add_executable(${TARGET} 
                "src/main.cpp"
                "src/Game.cpp"
                "src/SceneMain.cpp")

#链接库
target_link_libraries(${TARGET} 
                    ${SDL2_LIBRARIES}
                    SDL2_image::SDL2_image
                    SDL2_ttf::SDL2_ttf
                    SDL2_mixer::SDL2_mixer
                )
