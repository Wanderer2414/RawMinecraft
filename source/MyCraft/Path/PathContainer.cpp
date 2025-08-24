#include "PathContainer.h"
#include "Controller3D.h"
#include "Path.h"

namespace MyCraff {
    PathContainer::PathContainer()  {}
    PathContainer::~PathContainer() {}
    void PathContainer::insert(MyCraft::Path* path) {
        __paths.push_back(path);
    }
    bool PathContainer::handle(GLFWwindow* window) {
        bool is_changed = MyBase3D::Controller3D::handle(window);
        for (int i = __paths.size()-1; i>=0; i--) {
            is_changed = __paths[i]->handle(window) || is_changed;
            if (!__paths[i]->size()) {
                delete __paths[i];
                __paths.erase(__paths.begin()+i);
            }
        }
        return is_changed;
    }
}