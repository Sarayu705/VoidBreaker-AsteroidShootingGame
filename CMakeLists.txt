cmake_minimum_required(VERSION 3.16)
project(VoidBreaker LANGUAGES CXX)

# ==========================
# Compiler / Qt setup
# ==========================
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

# ==========================
# Source files
# ==========================
qt_add_executable(VoidBreaker
    ${CMAKE_SOURCE_DIR}/src/main.cpp
    ${CMAKE_SOURCE_DIR}/src/Constants.h

    ${CMAKE_SOURCE_DIR}/src/Game.h
    ${CMAKE_SOURCE_DIR}/src/Game.cpp

    ${CMAKE_SOURCE_DIR}/src/Player.h
    ${CMAKE_SOURCE_DIR}/src/Player.cpp

    ${CMAKE_SOURCE_DIR}/src/Bullet.h
    ${CMAKE_SOURCE_DIR}/src/Bullet.cpp

    ${CMAKE_SOURCE_DIR}/src/Asteroid.h
    ${CMAKE_SOURCE_DIR}/src/Asteroid.cpp

    ${CMAKE_SOURCE_DIR}/src/Mine.h
    ${CMAKE_SOURCE_DIR}/src/Mine.cpp

    ${CMAKE_SOURCE_DIR}/src/Explosion.h
    ${CMAKE_SOURCE_DIR}/src/Explosion.cpp

    ${CMAKE_SOURCE_DIR}/src/Debris.h
    ${CMAKE_SOURCE_DIR}/src/Debris.cpp

    ${CMAKE_SOURCE_DIR}/src/HUD.h
    ${CMAKE_SOURCE_DIR}/src/HUD.cpp

    ${CMAKE_SOURCE_DIR}/src/Star.h
    ${CMAKE_SOURCE_DIR}/src/Star.cpp

    ${CMAKE_SOURCE_DIR}/src/PowerUp.h
    ${CMAKE_SOURCE_DIR}/src/PowerUp.cpp
    resources.qrc
)

# ==========================
# Link libraries
# ==========================
target_link_libraries(VoidBreaker PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
)

# ==========================
# Optional: set app icon and bundle info (for macOS)
# ==========================
set_target_properties(VoidBreaker PROPERTIES
    MACOSX_BUNDLE TRUE
    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.VoidBreaker
    MACOSX_BUNDLE_BUNDLE_VERSION "1.0"
    MACOSX_BUNDLE_SHORT_VERSION_STRING "1.0"
)
