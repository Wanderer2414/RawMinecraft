// #include "gltf_mesh.h"

// GLTFStaticMesh::GLTFStaticMesh(const char* filename, float scale){
//     tinygltf::TinyGLTF loader;
//     std::string err;
//     std::string warn;

//     bool ret = loader.LoadASCIIFromFile(&model, &err, &warn, filename);
    
//     if (!warn.empty()) std::cerr << "Warning: " << warn << std::endl;
//     if (!err.empty()) throw std::runtime_error("Error: " + err);
//     if (!ret) throw std::runtime_error("Failed to load GLTF");

//     glGenVertexArrays(1, &VAO);
//     bindModel();
//     loadTextures();

// }
// GLTFStaticMesh::~GLTFStaticMesh() {
//     glDeleteVertexArrays(1, &VAO);
//     for (auto& buffer : buffers) {
//         glDeleteBuffers(1, &buffer.second);
//     }
//     for (auto texture : textures) {
//         glDeleteTextures(1, &texture);
//     }
// }
// void GLTFStaticMesh::bindModel() {
//     glBindVertexArray(VAO);
    
//     const auto& scene = model.scenes[model.defaultScene > -1 ? model.defaultScene : 0];
//     for (const auto nodeIndex : scene.nodes) {
//         bindModelNodes(model.nodes[nodeIndex]);
//     }
    
//     glBindVertexArray(0);
// }

// void GLTFStaticMesh::bindMesh(const tinygltf::Mesh& mesh) {
//     for (const auto& primitive : mesh.primitives) {
//         // Bind vertex attributes (POSITION, NORMAL, TEXCOORD_0)
//         for (const auto& attrib : primitive.attributes) {
//             const auto& accessor = model.accessors[attrib.second];
//             const auto& bufferView = model.bufferViews[accessor.bufferView];
//             const auto& buffer = model.buffers[bufferView.buffer];

//             if (buffers.find(accessor.bufferView) == buffers.end()) {
//                 GLuint vbo;
//                 glGenBuffers(1, &vbo);
//                 glBindBuffer(bufferView.target, vbo);
//                 glBufferData(bufferView.target, bufferView.byteLength,
//                              &buffer.data[bufferView.byteOffset], GL_STATIC_DRAW);
//                 buffers[accessor.bufferView] = vbo;
//             }

//             int attribLocation = -1;
//             if (attrib.first == "POSITION") attribLocation = 0;
//             else if (attrib.first == "NORMAL") attribLocation = 1;
//             else if (attrib.first == "TEXCOORD_0") attribLocation = 2;

//             if (attribLocation >= 0) {
//                 glEnableVertexAttribArray(attribLocation);
//                 glVertexAttribPointer(attribLocation, 
//                                     accessor.type == TINYGLTF_TYPE_VEC3 ? 3 : 2,
//                                     accessor.componentType,
//                                     accessor.normalized ? GL_TRUE : GL_FALSE,
//                                     bufferView.byteStride,
//                                     (void*)accessor.byteOffset);
//             }
//         }

//         // Bind indices
//         if (primitive.indices >= 0) {
//             const auto& accessor = model.accessors[primitive.indices];
//             const auto& bufferView = model.bufferViews[accessor.bufferView];
//             const auto& buffer = model.buffers[bufferView.buffer];

//             if (buffers.find(accessor.bufferView) == buffers.end()) {
//                 GLuint ebo;
//                 glGenBuffers(1, &ebo);
//                 glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
//                 glBufferData(GL_ELEMENT_ARRAY_BUFFER, bufferView.byteLength,
//                              &buffer.data[bufferView.byteOffset], GL_STATIC_DRAW);
//                 buffers[accessor.bufferView] = ebo;
//             }
//         }
//     }
// }
// void GLTFStaticMesh::bindModelNodes(const tinygltf::Node& node) {
//     // Xử lý mesh của node nếu có
//     if (node.mesh >= 0 && node.mesh < model.meshes.size()) {
//         bindMesh(model.meshes[node.mesh]);
//     }

//     // Xử lý transform của node
//     if (!node.matrix.empty()) {
//         // Node có matrix transform
//         glm::mat4 nodeMatrix = glm::make_mat4(node.matrix.data());
//         // Áp dụng transform vào shader hoặc lưu lại để sử dụng sau
//     } 
//     else {
//         // Node có transform riêng lẻ (translation, rotation, scale)
//         glm::vec3 translation = node.translation.empty() ? 
//             glm::vec3(0.0f) : glm::make_vec3(node.translation.data());
//         glm::quat rotation = node.rotation.empty() ? 
//             glm::quat(1.0f, 0.0f, 0.0f, 0.0f) : glm::make_quat(node.rotation.data());
//         glm::vec3 scale = node.scale.empty() ? 
//             glm::vec3(1.0f) : glm::make_vec3(node.scale.data());

//         // Tạo matrix transform từ các thành phần
//         glm::mat4 nodeMatrix = glm::translate(glm::mat4(1.0f), translation) *
//                               glm::mat4_cast(rotation) *
//                               glm::scale(glm::mat4(1.0f), scale);
//     }

//     // Xử lý các node con
//     for (const auto& childIndex : node.children) {
//         if (childIndex >= 0 && childIndex < model.nodes.size()) {
//             bindModelNodes(model.nodes[childIndex]);
//         }
//     }
// }

// void GLTFStaticMesh::loadTextures() {
//     for (const auto& texture : model.textures) {
//         const auto& image = model.images[texture.source];
        
//         GLuint texId;
//         glGenTextures(1, &texId);
//         glBindTexture(GL_TEXTURE_2D, texId);
        
//         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
//         GLenum format = GL_RGBA;
//         if (image.component == 3) format = GL_RGB;
        
//         glTexImage2D(GL_TEXTURE_2D, 0, format, image.width, image.height, 0,
//                     format, GL_UNSIGNED_BYTE, &image.image[0]);
        
//         textures.push_back(texId);
//     }
// }

// void GLTFStaticMesh::Draw() const {
//     glBindVertexArray(VAO);
    
//     for (const auto& mesh : model.meshes) {
//         for (const auto& primitive : mesh.primitives) {
//             // Bind texture
//             if (primitive.material >= 0) {
//                 const auto& material = model.materials[primitive.material];
//                 auto it = material.values.find("baseColorTexture");
//                 if (it != material.values.end() && !textures.empty()) {
//                     int texIndex = it->second.TextureIndex();
//                     if (texIndex >= 0 && texIndex < textures.size()) {
//                         glActiveTexture(GL_TEXTURE0);
//                         glBindTexture(GL_TEXTURE_2D, textures[texIndex]);
//                     }
//                 }
//             }
            
//             // Draw
//             if (primitive.indices >= 0) {
//                 const auto& accessor = model.accessors[primitive.indices];
//                 glDrawElements(primitive.mode, accessor.count,
//                               accessor.componentType, (void*)accessor.byteOffset);
//             } else {
//                 const auto& accessor = model.accessors[primitive.attributes.begin()->second];
//                 glDrawArrays(primitive.mode, 0, accessor.count);
//             }
//         }
//     }
    
//     glBindVertexArray(0);
// }