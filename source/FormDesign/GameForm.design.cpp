#include "GameForm.h"
#include "Block.h"
#include "Camera.h"
#include "Coordinate.h"
#include "GLFW/glfw3.h"
#include "Global.h"
namespace MyCraft {
    GameForm::GameForm() {
    }
    GameForm::~GameForm() {
    }
    void GameForm::run(GLFWwindow* window) {
        // Block block;
        // block.setPosition(0, 0, 0);
        // block.setType(BlockCatogary::Dirt);
        MyBase3D::Camera camera;
        Coordinate cor;
        
        while (!glfwWindowShouldClose(window)) {
            glClearColor(0, 0, 0, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            cor.glDraw();
            double x,y;
            glfwGetCursorPos(window, &x, &y);
            std::cout << x << " " << y << std::endl;
            glfwSwapBuffers(window);
            glfwPollEvents();
            if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
                glfwSetWindowShouldClose(window, true);
            }
        }
    }
}