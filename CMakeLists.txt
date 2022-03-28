cmake_minimum_required(VERSION 3.21)
project(game)

## If you want to link SFML statically
set(SFML_STATIC_LIBRARIES TRUE)

## In most cases better set in the CMake cache
set(SFML_DIR "SFML-2.5.1/lib/cmake/SFML")

find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)
add_executable(game main.cpp Player.cpp Player.h Animator.cpp Animator.h MainWindowFunctions.cpp MainWindowFunctions.h Enemy.cpp Enemy.h Bullet.cpp Bullet.h Node.cpp Node.h Constants.h Weapon.cpp Weapon.h MathFunctions.h MathFunctions.cpp)
target_link_libraries(game sfml-graphics sfml-audio)