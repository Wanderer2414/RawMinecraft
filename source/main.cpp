#include "General.h"
#include "Global.h"
#include "Application.h"
#include "MyCraft/ModelTest/gltf_mesh.h"

#include "MyCraft/ModelTest/GLTFModel.h"
#include "UI/Sound.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>


#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "tiny_gltf.h"

// int main() {
//     std::ios::sync_with_stdio(false);
//     std::cin.tie(0);
//     std::cout.tie(0);
//     auto sz = MyBase::getWindowSize();
//     MyCraft::Application application(sz.x, sz.y);
//     application.run();
//     return 0;
// }

int main(){
    std::cout<< "Starting Sound Test..." << std::endl;
    // Khởi tạo GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // Tạo cửa sổ
    GLFWwindow* window = glfwCreateWindow(800, 600, "Sound Test", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    // Khởi tạo hệ thống âm thanh (giả sử class Sound đã tự xử lý)
    Sound sound("assets/sounds/SoundEffect.mp3"); // Giả sử constructor không cần tham số
    // Hoặc nếu cần file: Sound sound("sound.wav");

    // Vòng lặp chính
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        // Xử lý input
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            sound.play();
            std::cout << "Playing sound" << std::endl;
        }
        
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
            sound.pause();
            std::cout << "Sound paused" << std::endl;
        }
        
        if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) {
            sound.stop();
            std::cout << "Sound stopped" << std::endl;
        }

        // Swap buffers và poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Dọn dẹp
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}