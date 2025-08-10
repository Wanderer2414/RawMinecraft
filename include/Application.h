#ifndef APPLICATION_H
#define APPLICATION_H
#include "Global.h"
#include "ModelTest/gltf_mesh.h"
namespace MyCraft {
    class Application {
        public:
            Application(const float& width, const float& height);
            ~Application();
            void run();
        private:
            GLFWwindow* __window;
            GLTFStaticMesh myModel;
    };
};
#endif