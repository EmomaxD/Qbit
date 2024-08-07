# Define directories for third-party libraries
set(GLEW_DIR "${CMAKE_SOURCE_DIR}/Engine/vendor/glew")
set(GLFW_DIR "${CMAKE_SOURCE_DIR}/Engine/vendor/glfw")
set(IMGUI_DIR "${CMAKE_SOURCE_DIR}/Engine/vendor/imgui")
set(ENTT_DIR "${CMAKE_SOURCE_DIR}/Engine/vendor/entt")
set(GLM_DIR "${CMAKE_SOURCE_DIR}/Engine/vendor/glm")
set(ASSIMP_DIR "${CMAKE_SOURCE_DIR}/Engine/vendor/assimp")
set(YAML_DIR "${CMAKE_SOURCE_DIR}/Engine/vendor/yaml-cpp")
set(IMGUIZMO_DIR "${CMAKE_SOURCE_DIR}/Engine/vendor/ImGuizmo")
set(BOX2D_DIR "${CMAKE_SOURCE_DIR}/Engine/vendor/Box2D")

# Glob source files
file(GLOB_RECURSE ENGINE_SRC
    "${CMAKE_SOURCE_DIR}/Engine/src/*.cpp"
    "${CMAKE_SOURCE_DIR}/Engine/src/*.hpp"
    "${CMAKE_SOURCE_DIR}/Engine/src/*.h"
)
file(GLOB IMGUI_SRC
    "${IMGUI_DIR}/*.cpp"
    "${IMGUI_DIR}/backends/imgui_impl_glfw.cpp"
    "${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp"
)
file(GLOB YAML_SRC
    "${YAML_DIR}/src/*.cpp"
    "${YAML_DIR}/src/*.h"
    "${YAML_DIR}/include/*.h"
)
file(GLOB IMGUIZMO_SRC
    "${IMGUIZMO_DIR}/*.cpp"
    "${IMGUIZMO_DIR}/*.h"
)

file (GLOB BOX2D_SRC
    "${BOX2D_DIR}/src/*/*.cpp"
    "${BOX2D_DIR}/include/*.h"
)


# Create Engine library
add_library(Engine 
    ${ENGINE_SRC}
    ${IMGUI_SRC}
    ${YAML_SRC}
    ${IMGUIZMO_SRC}
    ${BOX2D_SRC}
)

# Include directories
target_include_directories(Engine PRIVATE 
    ${CMAKE_SOURCE_DIR}/Engine
    ${CMAKE_SOURCE_DIR}/Engine/src
    ${CMAKE_SOURCE_DIR}/Engine/vendor
    ${CMAKE_SOURCE_DIR}/Engine/vendor/spdlog/include
    ${GLFW_DIR}/install/include 
    ${GLEW_DIR}/include
    ${IMGUI_DIR}
    ${IMGUI_DIR}/backends
    ${GLM_DIR}/glm
    ${GLM_DIR}
    ${ENTT_DIR}/src/entt
    ${ASSIMP_DIR}/include
    ${YAML_DIR}/include
    ${IMGUIZMO_DIR}
    ${BOX2D_DIR}/include
)

# Compile definitions
target_compile_definitions(Engine PRIVATE GLEW_STATIC YAML_CPP_STATIC_DEFINE)

# Define QB_DEBUG for Debug configuration only
target_compile_definitions(Engine PRIVATE 
    $<$<CONFIG:Debug>:QB_DEBUG>
    _CRT_SECURE_NO_WARNINGS
)

# Link libraries based on the operating system
if(WIN32)
    target_precompile_headers(Engine PRIVATE "${CMAKE_SOURCE_DIR}/Engine/src/qbpch.h")
    target_compile_definitions(Engine PRIVATE QB_PLATFORM_WINDOWS GLFW_INCLUDE_NONE)

    

    target_link_directories(Engine PRIVATE 
        ${GLFW_DIR}/install/lib
        ${GLEW_DIR}/Windows/lib/Release/x64
        ${ASSIMP_DIR}/lib/Debug/
        ${ASSIMP_DIR}/contrib/zlib/Debug
    )
    target_link_libraries(Engine PRIVATE 
        glew32s 
        glfw3 
        opengl32 
        gdi32 
        winmm
        assimp-vc143-mtd
        zlibstaticd
    )
elseif(UNIX)
    target_link_directories(Engine PRIVATE 
        ${GLFW_DIR}/install/lib 
        ${GLEW_DIR}/lib
    )
    target_link_libraries(Engine PRIVATE 
        glfw3 
        GLEW 
        m 
        dl 
        pthread 
        GL
    )
else()
    message(FATAL_ERROR "Unsupported operating system")
endif()

# Set the output directory for the executable if needed
#set_target_properties(Engine PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/Out")
