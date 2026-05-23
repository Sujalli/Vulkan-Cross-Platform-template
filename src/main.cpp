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
        glfwCreateWindow(
            WIDTH,
            HEIGHT,
            "Vulkan GLFW",
            nullptr,
            nullptr
        );

    uint32_t extensionCount = 0;

    vkEnumerateInstanceExtensionProperties(
        nullptr,
        &extensionCount,
        nullptr
    );

    std::cout
        << "Available Vulkan extensions: "
        << extensionCount
        << std::endl;

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}