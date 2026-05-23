# Vulkan Cross Platform Setup Guide

Complete Vulkan setup for **macOS (MoltenVK)**, **Linux**, and **Windows** using the Vulkan SDK, GLFW, GLM, CMake, and VSCode.

---

## Table of Contents

1. [Project Structure](#1-project-structure)
2. [Installing Dependencies](#2-installing-dependencies)
3. [Vulkan SDK Notes](#3-vulkan-sdk-notes)
4. [GLFW Setup](#4-glfw-setup)
5. [GLM Setup](#5-glm-setup)
6. [CMake Setup](#6-cmake-setup)
7. [Example Source Code](#7-example-source-code)
8. [Build Instructions](#8-build-instructions)
9. [VSCode Configuration](#9-vscode-configuration)
10. [Platform Notes](#10-platform-notes)
11. [Common Errors](#11-common-errors)
12. [Recommended Libraries](#12-recommended-libraries)
13. [Recommended Vulkan Learning Order](#13-recommended-vulkan-learning-order)
14. [Resources](#14-resources)

---

## 1. Project Structure

```
Vulkan_tutorial/
│
├── CMakeLists.txt
├── README.md
│
├── src/
│   └── main.cpp
│
├── include/
├── lib/
├── external/
├── shaders/
├── assets/
├── build/
│
└── .vscode/
    ├── tasks.json
    └── launch.json
```

---

## 2. Installing Dependencies

### macOS

Apple does not support native Vulkan. Vulkan works through MoltenVK:

```
Vulkan → MoltenVK → Metal → macOS GPU Driver
```

**Install Xcode Command Line Tools**
```bash
xcode-select --install
```

**Install Homebrew** — https://brew.sh
```bash
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
```

**Install GLFW and GLM**
```bash
brew install glfw glm
```

**Install Vulkan SDK**

Download from https://vulkan.lunarg.com/sdk/home and install the `.dmg`. Then:
```bash
source ~/.zshrc
vulkaninfo   # verify — should print Vulkan device info
```

---

### Linux (Ubuntu / Debian)

```bash
sudo apt update
sudo apt install \
    build-essential \
    cmake \
    gdb \
    clang \
    libglfw3-dev \
    libglm-dev \
    vulkan-tools \
    libvulkan-dev
```

**Verify:**
```bash
vulkaninfo
```

---

### Windows

**Recommended tools:** Visual Studio 2022, Vulkan SDK, VSCode, CMake, vcpkg

**Install Vulkan SDK** from https://vulkan.lunarg.com/sdk/home, then verify:
```powershell
vulkaninfo
```

**Install vcpkg** — https://github.com/microsoft/vcpkg
```powershell
git clone https://github.com/microsoft/vcpkg
cd vcpkg
.\bootstrap-vcpkg.bat
.\vcpkg install glfw3
.\vcpkg install glm
```

---

## 3. Vulkan SDK Notes

### macOS

The SDK includes MoltenVK, the Vulkan Loader, and Validation Layers.

```bash
echo $VULKAN_SDK
# Expected: /Users/yourname/VulkanSDK/1.x.xxx/macOS
```

If the variable is empty, add to `~/.zshrc`:
```bash
export VULKAN_SDK=~/VulkanSDK/1.x.xxx/macOS
export PATH=$VULKAN_SDK/bin:$PATH
```
Then reload: `source ~/.zshrc`

### Windows

The SDK installer sets environment variables automatically.
```powershell
echo $env:VULKAN_SDK
```

### Linux

Usually installed globally through the package manager.

---

## 4. GLFW Setup

| Platform | Command |
|---|---|
| macOS | `brew install glfw` |
| Ubuntu/Debian | `sudo apt install libglfw3-dev` |
| Windows (vcpkg) | `.\vcpkg install glfw3` |

---

## 5. GLM Setup

GLM is header-only.

| Platform | Command |
|---|---|
| macOS | `brew install glm` |
| Ubuntu/Debian | `sudo apt install libglm-dev` |
| Windows (vcpkg) | `.\vcpkg install glm` |

---

## 6. CMake Setup

Create `CMakeLists.txt` in the project root:

```cmake
cmake_minimum_required(VERSION 3.20)

project(Vulkan_tutorial)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Vulkan
find_package(Vulkan REQUIRED)

# GLFW
find_package(glfw3 REQUIRED)

# GLM
find_package(glm REQUIRED)

# Source Files
set(SOURCES
    src/main.cpp
)

# Executable
add_executable(Vulkan_tutorial ${SOURCES})

# Include Directories
target_include_directories(Vulkan_tutorial PRIVATE
    include
)

# macOS Specific
if(APPLE)
    target_compile_definitions(Vulkan_tutorial PRIVATE
        VK_ENABLE_BETA_EXTENSIONS
    )
    target_link_libraries(Vulkan_tutorial PRIVATE
        Vulkan::Vulkan
        glfw
        glm::glm
    )
endif()

# Linux Specific
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

# Windows Specific
if(WIN32)
    target_link_libraries(Vulkan_tutorial PRIVATE
        Vulkan::Vulkan
        glfw
        glm::glm
    )
endif()
```

---

## 7. Example Source Code

Create `src/main.cpp`:

```cpp
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>

const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;

int main() {

    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    GLFWwindow* window =
        glfwCreateWindow(WIDTH, HEIGHT, "Vulkan GLFW Window", nullptr, nullptr);

    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

    std::cout << "Available Vulkan Extensions: " << extensionCount << std::endl;

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
```

---

## 8. Build Instructions

### macOS / Linux

```bash
mkdir build
cd build
cmake ..
make
./Vulkan_tutorial
```

### Windows

```powershell
mkdir build
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=[vcpkg path]/scripts/buildsystems/vcpkg.cmake
cmake --build .
.\Debug\Vulkan_tutorial.exe
```

---

## 9. VSCode Configuration

### `.vscode/tasks.json`

```json
{
    "version": "2.0.0",
    "tasks": [
        {
            "label": "Build Vulkan",
            "type": "shell",
            "command": "cd build && make",
            "group": {
                "kind": "build",
                "isDefault": true
            }
        }
    ]
}
```

### `.vscode/launch.json`

```json
{
    "version": "0.2.0",
    "configurations": [
        {
            "name": "Launch Vulkan",
            "type": "cppdbg",
            "request": "launch",
            "program": "${workspaceFolder}/build/Vulkan_tutorial",
            "args": [],
            "cwd": "${workspaceFolder}",
            "stopAtEntry": false,
            "MIMode": "lldb"
        }
    ]
}
```

---

## 10. Platform Notes

### macOS
- Required Vulkan portability extension: `VK_KHR_portability_enumeration`
- MoltenVK is mandatory

### Linux
- X11 libraries are required
- Wayland support may need additional packages

### Windows
- Use either the Visual Studio Generator or the Ninja Generator
- Recommended compiler: MSVC

---

## 11. Common Errors

**`"No SOURCES given to target"`**
`src/main.cpp` does not exist. Ensure `Vulkan_tutorial/src/main.cpp` is present.

**`"Could NOT find Vulkan"`**
Install the Vulkan SDK and verify with `vulkaninfo`.

**`"Undefined symbols"`**
Usually missing libraries in `target_link_libraries()`.

**GLFW window opens then immediately closes**
Likely causes: Vulkan initialization failure, validation layer issues, or a missing portability extension on macOS.

---

## 12. Recommended Libraries

| Library | Purpose |
|---|---|
| GLM | Mathematics |
| Dear ImGui | GUI |
| stb_image | Texture Loading |
| tinyobjloader | OBJ Loading |
| spdlog | Logging |
| RenderDoc | GPU Debugging |

---

## 13. Recommended Vulkan Learning Order

1. Window Creation
2. Vulkan Instance
3. Validation Layers
4. Physical Device
5. Logical Device
6. Swapchain
7. Render Pass
8. Graphics Pipeline
9. Command Buffers
10. Vertex Buffers
11. Uniform Buffers
12. Textures
13. Descriptor Sets
14. Synchronization
15. Compute Shaders

---

## 14. Resources

- **Vulkan Tutorial** — https://vulkan-tutorial.com
- **Khronos Vulkan Docs** — https://docs.vulkan.org
- **GLFW Documentation** — https://www.glfw.org/docs/latest/
- **MoltenVK** — https://github.com/KhronosGroup/MoltenVK
- **Vulkan Samples** — https://github.com/KhronosGroup/Vulkan-Samples