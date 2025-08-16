#include "General.h"
#include "Global.h"
#include "Application.h"
#include "MyCraft/ModelTest/gltf_mesh.h"

#include "MyCraft/ModelTest/GLTFModel.h"
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

// Kích thước cửa sổ
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// Biến camera
// CAMERA UP VECTOR IS FOLLOWING TO Z-AXIS
// TAKE CARE OF TRUE CAMERA POSITION AND CAMERA DIRECTION
glm::vec3 cameraPos = glm::vec3(-1.0f, -1.0f, 0.0f);
glm::vec3 cameraFront = glm::vec3(1.0f, 1.0f, 0.f);
glm::vec3 cameraUp = glm::vec3(0.0f, 0.f, 1.0f);
float yaw = -90.0f;
float pitch = 0.0f;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
float fov = 45.0f;

// Thời gian
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// Hàm callback xử lý input
void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    float cameraSpeed = 2.5f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

// Hàm callback chuột
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
}

// Hàm callback scroll
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    fov -= (float)yoffset;
    if (fov < 1.0f)
        fov = 1.0f;
    if (fov > 45.0f)
        fov = 45.0f;
}

int main() {
    // Khởi tạo GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Tạo cửa sổ
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "GLTFModel Example", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    // Khởi tạo GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        return -1;
    }

    // In thông tin OpenGL
    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;

    // Cấu hình OpenGL
    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

    // Tải model
    std::cout << "Loading model..." << std::endl;
    try {
        GLTFModel myModel("assets/models/pig_model.gltf", 1.0f);
        myModel.setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
        std::cout << "Model loaded successfully!" << std::endl;

        // Vòng lặp chính
        while (!glfwWindowShouldClose(window)) {
            std::cout<< "Frame start" << std::endl;
            float currentFrame = glfwGetTime();
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;
            std::cout << "Delta Time: " << deltaTime << " seconds" << std::endl;

            processInput(window);

            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            std::cout << "Drawing model..." << std::endl;
            glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
            glm::mat4 projection = glm::perspective(glm::radians(fov), 
                (float)SCR_WIDTH/(float)SCR_HEIGHT, 0.1f, 100.0f);
            std::cout << "View Matrix: " <<  std::endl;
            std::cout << "Projection Matrix: " << std::endl;
            myModel.rotate(glm::radians(20.0f * deltaTime), glm::vec3(0.0f, 1.0f, 0.0f));
            std::cout << "Drawing model with matrices..." << std::endl;
            myModel.draw(glm::mat4(1.0f), view, projection);
            std::cout << "Model drawn successfully!" << std::endl;

            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwTerminate();
    return 0;
}