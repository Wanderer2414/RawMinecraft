#ifndef DISTRIBUTION_H
#define DISTRIBUTION_H
#include "Global.h"
namespace MyCraft {
    class BvhLightTree {
    public:
        BvhLightTree();
        ~BvhLightTree();

        void insert(const glm::vec3& position, const float& radius);
        float remove(const glm::vec3& position);
        void print();
        float getLightIndensity(const glm::vec3& position) const;
    private:
        struct Node {
            unsigned char height;
            float radius;
            glm::vec3 position;
            Node* right, *left;
            Node();
            ~Node();
            Node(const glm::vec3& position, const float& radius);
            bool operator-(const Node& node);
            Node operator+(const Node& node);
        };
        Node* __root;
        void __insert(Node*& root, Node* node);
        float __remove(Node*& root, const glm::vec3& position);
        void __rotateRight(Node*& root);
        void __rotateLeft(Node*& root);
        void __print(Node*& root);
        float __getIndensity(Node* root, const glm::vec3& position) const;
    };
}
#endif