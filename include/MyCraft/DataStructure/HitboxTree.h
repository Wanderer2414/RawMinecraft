#ifndef HITBOX_TREE_H
#define HITBOX_TREE_H
#include "ModelController.h"
#include "Ray.h"
namespace MyCraft {
    class HitboxTree;
    class HitboxNode {
    public:
        HitboxNode(HitboxTree* tree);
        ~HitboxNode();
        HitboxNode* parent, *left, *right;
        int height;
        bool contains(const glm::vec3& position) const;
        bool isCollistion(const MyBase3D::Ray3f& ray) const;
        virtual void setShape(const glm::mat4x3&);
        virtual glm::mat4x3 getShape() const;
        void update();
        float operator-(const HitboxNode& node) const;
        HitboxNode operator+(const HitboxNode& node) const;
        friend std::ostream& operator<<(std::ostream& cout, const HitboxNode& node);
    protected:
    private:
        glm::mat4x3 __shape;
        glm::vec2   x, y, z;
        HitboxTree* tree;
    };

    class HitboxTree {
    public:
        HitboxTree();
        ~HitboxTree();
        HitboxTree(const HitboxTree&) = delete;
        HitboxTree& operator=(const HitboxTree&) const = delete; 

        HitboxNode* get(const glm::vec3& position);
        void insert(HitboxNode* model);
        void remove(HitboxNode* model);
        void update(HitboxNode* node);
        void print() const;
        
    protected:
    private:
        HitboxNode* __root;

        void __insert(HitboxNode*& root, HitboxNode* model);
        bool __remove(HitboxNode*& root, HitboxNode* model);
        void __rotateLeft(HitboxNode*& root);
        void __rotateRight(HitboxNode*& root);
        void __print(HitboxNode* root) const;
    };
}
#endif