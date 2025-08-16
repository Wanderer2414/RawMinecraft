#include "LightDistribution.h"
#include "glm/geometric.hpp"
#include <format>

namespace MyCraft {
    BvhLightTree::BvhLightTree(): __root(0) {}
    BvhLightTree::~BvhLightTree() {
        if (__root) delete __root;
        __root = 0;
    }

    void BvhLightTree::insert(const glm::vec3& position, const float& radius) {
        __insert(__root, new Node(position, radius));
    }
    float BvhLightTree::remove(const glm::vec3& position) {
        return __remove(__root, position);
    }
    void BvhLightTree::print() {__print(__root);};
    float BvhLightTree::getLightIndensity(const glm::vec3& position) const {
        return  __getIndensity(__root, position);
    }
            
    BvhLightTree::Node::Node(): right(0), left(0), height(0) {}
    BvhLightTree::Node::Node(const glm::vec3& p, const float& r): position(p), radius(r), right(0), left(0), height(0) {}
    bool BvhLightTree::Node::operator-(const Node& node) {
        return glm::length(position - node.position)<=radius+node.radius;
    }
    BvhLightTree::Node::~Node() {
        if (left) delete left;
        if (right) delete right;
        left = right = 0;
    }
    BvhLightTree::Node BvhLightTree::Node::operator+(const Node& node) {
        Node ans;
        glm::vec3 delta = node.position - position;
        float distance = glm::length(delta);
        ans.radius = (distance + radius + node.radius)/2;
        ans.position = position + glm::normalize(delta)*float(ans.radius-radius);
        return ans;
    }
    void BvhLightTree::__insert(Node*& root, Node* node) {
        if (!root) root = node;
        else if (!root->height) {
            if (root->position != node->position) {
                Node* tmp = new Node();
                *tmp = *root + *node;
                tmp->left = node;
                tmp->right = root;
                tmp->height = 1;
                root = tmp;
            }
        }
        else if (glm::length(root->left->position - node->position)>glm::length(root->right->position - node->position)) {
            __insert(root->right, node);
            Node tmp = *root->right + *root->left;
            tmp.left = root->left;
            tmp.right = root->right;
            tmp.height = std::max(tmp.left->height, tmp.right->height)+1;
            *root = tmp;
            tmp.left = tmp.right = 0;
            if (root->right->height - root->left->height >= 2) {
                if (root->right->left->height>root->right->right->height) __rotateRight(root->right);
                __rotateLeft(root);
            }
            root->height = std::max(root->left->height, root->right->height)+1;
        }
        else {
            __insert(root->left, node);
            Node tmp = *root->right + *root->left;
            tmp.left = root->left;
            tmp.right = root->right;
            tmp.height = std::max(tmp.left->height, tmp.right->height)+1;
            *root = tmp;
            tmp.left = tmp.right = 0;
            if (root->left->height - root->right->height >= 2) {
                if (root->left->right->height>root->left->left->height) __rotateLeft(root->left);
                __rotateRight(root);
            }
            root->height = std::max(root->left->height, root->right->height)+1;
        }
    }
    void BvhLightTree::__rotateLeft(Node*& root) {
        Node* tmp = root->right;
        root->right = tmp->left;
        {
            Node merge = *root->right + *root->left;
            merge.left = root->left;
            merge.right = root->right;
            merge.height = std::max(root->left->height, root->right->height) + 1;
            *root = merge;
            merge.left = merge.right = 0;
        }
        tmp->left = root;
        root = tmp;
        {
            Node merge = *root->right + *root->left;
            merge.left = root->left;
            merge.right = root->right;
            merge.height = std::max(root->left->height, root->right->height) + 1;
            *root = merge;
            merge.left = merge.right = 0;
        }
    }
    void BvhLightTree::__rotateRight(Node*& root) {
        Node* tmp = root->left;
        root->left = tmp->right;
        {
            Node merge = *root->right + *root->left;
            merge.left = root->left;
            merge.right = root->right;
            merge.height = std::max(root->left->height, root->right->height) + 1;
            *root = merge;
            merge.left = merge.right = 0;
        }
        tmp->right = root;
        root = tmp;
        {
            Node merge = *root->right + *root->left;
            merge.left = root->left;
            merge.right = root->right;
            merge.height = std::max(root->left->height, root->right->height) + 1;
            *root = merge;
            merge.left = merge.right = 0;
        }
    }
    float BvhLightTree::__remove(Node*& root, const glm::vec3& position) {
        if (!root) return 0;
        else if (!root->height) {
            if (position == root->position) {
                float distance = root->radius;
                delete root;
                root = 0;
                return distance;
            }
            return 0;
        }
        else {
            if (glm::length(root->right->position - position)<root->right->radius) {
                float distance = __remove(root->right, position);
                if (!root->right) {
                    Node* tmp = root->left;
                    root->left = 0;
                    delete root;
                    root = tmp;
                    return distance;
                }
                else if (distance) {
                    Node tmp = *root->left + *root->right;
                    tmp.left = root->left;
                    tmp.right = root->right;
                    tmp.height = std::max(root->left->height, root->right->height)+1;
                    *root = tmp;
                    tmp.right = tmp.left = 0;
                    
                    if (root->left->height - root->right->height >= 2) {
                        if (root->left->right->height>root->left->left->height) __rotateLeft(root->left);
                        __rotateRight(root);
                    }
                    root->height = std::max(root->left->height, root->right->height)+1;
                    return distance;
                }
            }
            if (glm::length(root->left->position - position) < root->left->radius) {
                float distance = __remove(root->left, position);
                if (!root->left) {
                    Node* tmp = root->right;
                    root->right = 0;
                    delete root;
                    root = tmp;
                    return distance;
                }
                else if (distance) {
                    Node tmp = *root->right + *root->left;
                    tmp.left = root->left;
                    tmp.right = root->right;
                    tmp.height = std::max(root->left->height, root->right->height)+1;
                    *root = tmp;
                    tmp.right = tmp.left = 0;

                    if (root->right->height - root->left->height >= 2) {
                        if (root->right->left->height>root->right->right->height) __rotateRight(root->right);
                        __rotateLeft(root);
                    }
                    root->height = std::max(root->left->height, root->right->height)+1;
                    return distance;
                }
            }
            return 0;
        }
    }
    void BvhLightTree::__print(Node*& root) {
        if (!root) return ;
        std::cout.width(root->height*2);
        std::cout << " ";
        std::cout << std::format("({},{},{}) R = {}", root->position.x, root->position.y, root->position.z, root->radius) << std::endl;
        if (root->left) __print(root->left);
        if (root->right) __print(root->right);
    }
    float BvhLightTree::__getIndensity(Node* root, const glm::vec3& position) const {
        if (!root) return 0;
        if (!root->height) {
            float I = root->radius - glm::length(root->position - position);
            if (I>0) return I;
            else return 0;
        }
        else {
            float I = 0;
            if (glm::length(root->left->position - position) < root->left->radius) {
                I = std::max(I, __getIndensity(root->left, position));
            }
            if (glm::length(root->right->position - position) < root->right->radius) {
                I = std::max(I, __getIndensity(root->right, position));
            }
            return I;
        }
    }
}