cmake_minimum_required(VERSION 3.20)

project(Vulkan_tutorial)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# -----------------------------
# Vulkan
# -----------------------------
find_package(Vulkan REQUIRED)

# -----------------------------
# GLFW
# -----------------------------
find_package(glfw3 REQUIRED)

# -----------------------------
# GLM
# -----------------------------
find_package(glm REQUIRED)

# -----------------------------
# Source Files
# -----------------------------
set(SOURCES
    src/main.cpp
)

# -----------------------------
# Executable
# -----------------------------
add_executable(Vulkan_tutorial ${SOURCES})

# -----------------------------
# Include Directories
# -----------------------------
target_include_directories(Vulkan_tutorial PRIVATE
    include
)

# -----------------------------
# macOS Specific
# -----------------------------
if(APPLE)

    # MoltenVK portability extensions
    target_compile_definitions(Vulkan_tutorial PRIVATE
        VK_ENABLE_BETA_EXTENSIONS
    )

endif()

# -----------------------------
# Linux Specific
# -----------------------------
if(UNIX AND NOT APPLE)

    target_link_libraries(Vulkan_tutorial PRIVATE
        Vulkan::Vulkan
        glfw
        glm::glm
        dl
        pthread
        X11
        Xxf86vm
        Xrandr
        Xi
    )

endif()

# -----------------------------
# Windows Specific
# -----------------------------
if(WIN32)

    target_link_libraries(Vulkan_tutorial PRIVATE
        Vulkan::Vulkan
        glfw
        glm::glm
    )

endif()

# -----------------------------
# macOS Linking
# -----------------------------
if(APPLE)

    target_link_libraries(Vulkan_tutorial PRIVATE
        Vulkan::Vulkan
        glfw
        glm::glm
    )

endif()