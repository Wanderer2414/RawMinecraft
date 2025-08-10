// #pragma once
// #include "Extended/tiny_gltf.h"
// #include <GL/glew.h>
// #include <glm/glm.hpp>
// #include <vector>
// #include <map>

// class GLTFStaticMesh {
// public:
//     tinygltf::Model model;
//     std::vector<GLuint> VAOs; // VAO cho mỗi primitive
//     std::map<int, GLuint> buffers; // Lưu VBO và EBO
//     std::vector<GLuint> textures; // Lưu texture
//     std::vector<glm::mat4> nodeTransforms; // Transform của mỗi node
//     float scale;
//     glm::vec3 position; // Vị trí thực thể trong thế giới

//     GLTFStaticMesh(const char* filename, float scale = 1.0f);
//     ~GLTFStaticMesh();

//     void Draw(GLuint shaderProgram, const glm::mat4& view, const glm::mat4& projection, float time);

// private:
//     void loadTextures();
//     void bindModel(size_t& vaoIndex);
//     void bindMesh(const tinygltf::Mesh& mesh, size_t& vaoIndex);
//     void bindModelNodes(const tinygltf::Node& node, size_t& vaoIndex);
//     void applyAnimation(float time);
// };
