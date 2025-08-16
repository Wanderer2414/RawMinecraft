#pragma once
#include "config.h"
#include "Extended/tiny_gltf.h"

class GLTFStaticMesh {
public:
    tinygltf::Model model;
    std::pair<unsigned int, std::map<int, unsigned int>> VAO_and_EBOs;
    float scale;
    std::pair<unsigned int, std::map<int, unsigned int>> bindModel();
    float prepareForDrawing();
    void draw();

    GLTFStaticMesh(const char* filename, float scale = 1.0f);
    ~GLTFStaticMesh();
    
private:
    void bindModelNodes(std::map<int, unsigned int> & ebos, tinygltf::Node &);
    void bindMesh(std::map<int, unsigned int> & ebos, tinygltf::Mesh & mesh);
    void drawModelNodes(tinygltf::Node & node);
    void drawMesh(const std::map<int, unsigned int> & ebos, tinygltf::Mesh & mesh);
};
