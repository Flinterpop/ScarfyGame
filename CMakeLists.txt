cmake_minimum_required(VERSION 3.24) # FetchContent with integrated find_package() is available in 3.24+
project(Scarfy)

# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


# Disable usless compiler warnings
if (MSVC)
	add_compile_options(/wd4068) # Disable unknown pragma warning. https://docs.microsoft.com/en-us/cpp/error-messages/compiler-warnings/compiler-warning-level-1-c4068
	add_compile_options(/wd4996) # Disable unsecure function warnings (e.g., sprintf()
endif()


# Dependencies
include(FetchContent)

set(RAYLIB_VERSION 4.0.0)
FetchContent_Declare(
    raylib
    URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
    FIND_PACKAGE_ARGS ${RAYLIB_VERSION} EXACT
)
set(BUILD_EXAMPLES OFF CACHE INTERNAL "")
FetchContent_MakeAvailable(raylib)

set(RAYGUI_VERSION 3.2)
FetchContent_Declare(
    raygui
    URL https://github.com/raysan5/raygui/archive/refs/tags/${RAYGUI_VERSION}.tar.gz
    FIND_PACKAGE_ARGS ${RAYGUI_VERSION} EXACT
)
set(BUILD_EXAMPLES OFF CACHE INTERNAL "")
FetchContent_MakeAvailable(raygui)
add_subdirectory(${raygui_SOURCE_DIR}/projects/CMake)

set(BOX2D_BUILD_UNIT_TESTS OFF CACHE INTERNAL "")
set(BOX2D_BUILD_TESTBED OFF CACHE INTERNAL "")
add_subdirectory(ThirdParty/box2d)


include_directories(main)
# Our Project
set(SOURCE_FILES
	main/Main.cpp
	main/Actor.cpp
	main/Collectable.cpp
	main/CharacterActor.cpp
	main/CommandListener.cpp
	main/DisplayOverlay.cpp
	main/DrawPhysics.cpp
	main/GameOverlay.cpp
	main/InputHandler.cpp
	main/MainMenuScene.cpp
	main/PhysicsObject.cpp
	main/PhysicsContactHandler.cpp
	main/Scarfy.cpp
	main/ScarfyScene.cpp
	main/Scene.cpp
	main/TextureAtlas.cpp
	main/TileMap2D.cpp 
	main/TileMap2DLoader.cpp 
	main/TileMap2DScene.cpp
	main/TrackingCamera2D.cpp)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
#set(raylib_VERBOSE 1)
target_link_libraries(${PROJECT_NAME} raylib)
target_link_libraries(${PROJECT_NAME} raygui)
target_link_libraries(${PROJECT_NAME} box2d)
target_include_directories(${PROJECT_NAME} PUBLIC ThirdParty/box2d/include)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)
target_include_directories(${PROJECT_NAME} PUBLIC . ThirdParty/raylib-cpp/include ThirdParty/tileson/include ThirdParty/pocketlzma)

set_property(TARGET Scarfy PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Scarfy)

# Create a symbolic link to the data directory after the project is built
ADD_CUSTOM_TARGET(link_to_data ALL
				  DEPENDS ${PROJECT_NAME}
                  COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_SOURCE_DIR}/Data/ $<TARGET_FILE_DIR:${PROJECT_NAME}>/Data/)


# Web Configurations
if (${PLATFORM} STREQUAL "Web")
    # Tell Emscripten to build an example.html file.
    set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".html")
endif()

# Checks if OSX and links appropriate frameworks (Only required on MacOS)
if (APPLE)
    target_link_libraries(${PROJECT_NAME} "-framework IOKit")
    target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
endif()