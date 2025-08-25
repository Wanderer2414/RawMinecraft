#ifndef PATH_CONTAINER_H
#define PATH_CONTAINER_H
#include "Container3D.h"
#include "Controller3D.h"
#include "Path.h"
namespace MyCraff {
    class PathContainer: public MyBase3D::Container3D {
    public:
        PathContainer();
        ~PathContainer();
        PathContainer(const PathContainer&) = delete;
        PathContainer& operator=(const PathContainer&) const = delete; 
        void insert(MyCraft::Path* path);
    protected:
    private:
        std::vector<MyCraft::Path*> __paths;
        bool handle(GLFWwindow* window) override;
    };
}
#endif