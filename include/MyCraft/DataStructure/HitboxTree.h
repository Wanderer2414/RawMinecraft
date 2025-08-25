#ifndef HITBOX_TREE_H
#define HITBOX_TREE_H
#include "Global.h"
namespace MyCraft {
    class HitboxTree;
    class HitboxNode {
    public:
        HitboxNode(HitboxTree* tree);
        virtual ~HitboxNode();
        HitboxNode* parent, *left, *right;
        int height;
        bool contains(const glm::vec3& position) const;
        float isCollistion(const glm::vec3& postiion, const glm::vec3& dir) const;
        virtual void setShape(const glm::mat4x3&);
        virtual glm::mat4x3 getShape() const;
        void update();
        float operator-(const HitboxNode& node) const;
        HitboxNode operator+(const HitboxNode& node) const;
        friend std::ostream& operator<<(std::ostream& cout, const HitboxNode& node);
        friend class HitboxTree;
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

        std::vector<HitboxNode*> get(const glm::vec3& position) const;
        HitboxNode* get(const glm::vec3& position, const glm::vec3& direction) const;
        void insert(HitboxNode* model);
        void remove(HitboxNode* model);
        void update(HitboxNode* node);
        void print() const;
        
        class Iterator {
        public:
            Iterator(HitboxNode*);
            HitboxNode* operator*();
            Iterator& operator++();
            bool operator==(const Iterator& iter) const;
            bool operator!=(const Iterator& iter) const;
        private:
            HitboxNode* __node;
        };

        Iterator begin() const, end() const;
    protected:
    private:
        HitboxNode* __root;

        void __insert(HitboxNode*& root, HitboxNode* model);
        bool __remove(HitboxNode*& root, HitboxNode* model);
        void __rotateLeft(HitboxNode*& root);
        void __rotateRight(HitboxNode*& root);
        void __print(HitboxNode* root) const;
        void __get(std::vector<HitboxNode*>&, HitboxNode* root, const glm::vec3& position) const;
        std::pair<HitboxNode*, float> __get(HitboxNode* root, const glm::vec3& position, const glm::vec3& dir) const;
    };
}
#endif