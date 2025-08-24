#include "HitboxTree.h"
#include "Ray.h"
#include <limits>
#include <pthread.h>
namespace MyCraft {
    HitboxNode::HitboxNode(HitboxTree* t): parent(0),left(0), right(0), height(0), tree(t) {}
    HitboxNode::~HitboxNode() {
        if (left) delete left;
        if (right) delete right;
        left = right = 0;
    }

    std::ostream& operator<<(std::ostream& cout, const HitboxNode& root) {
        return cout << "(" << root.x.x << " " << root.x.y << ", " << root.y.x << " " << root.y.y << ", " << root.z.x << " " << root.z.y << ")" ;
    }

    HitboxTree::Iterator::Iterator(HitboxNode * node): __node(node) {}
    HitboxNode* HitboxTree::Iterator::operator*() {
        return __node;
    }

    HitboxTree::Iterator& HitboxTree::Iterator::operator++() {
        if (!__node || !__node->parent) __node = 0;
        else if (__node->parent->right != __node) __node = __node->parent->right;
        else {
            while (__node->parent && __node->parent->right == __node) __node = __node->parent;
            if (!__node->parent) __node = 0;
            else {
                __node = __node->parent->right;
                while (__node->left) __node = __node->left;
            }
        }
        return *this;
    }
    bool HitboxTree::Iterator::operator==(const Iterator& iter) const {
        return iter.__node == __node;
    }
    bool HitboxTree::Iterator::operator!=(const Iterator& iter) const {
        return iter.__node != __node;
    }

    HitboxTree::Iterator HitboxTree::begin() const {
        if (!__root) return 0;
        HitboxNode* node = __root;
        while (node->left) node = node->left;
        return node;
    } 
    HitboxTree::Iterator HitboxTree::end() const {
        return 0;
    }

    bool HitboxNode::contains(const glm::vec3& position) const {
        glm::vec3 offset = position-__shape[0];
        bool contains = true;
        float length = glm::length(__shape[1]);
        float dummy = glm::dot(offset,__shape[1]);
        contains = contains && dummy<=length*length && dummy>=0;
        length = glm::length(__shape[2]);
        dummy = glm::dot(offset, __shape[2]);
        contains = contains && dummy<=length*length && dummy>=0;
        length = glm::length(__shape[3]);
        dummy = glm::dot(offset, __shape[3]);
        contains = contains && dummy<=length*length && dummy>=0;
        return contains;
    }
    float HitboxNode::isCollistion(const glm::vec3& position, const glm::vec3& direction) const {
        glm::vec3 origin = position - __shape[0];
        bool contains = false;
        float ans = std::numeric_limits<float>::max();
        {
            glm::mat3 mat(__shape[1], __shape[2], - direction);
            glm::vec3 point = glm::inverse(mat)*origin;
            contains = (point.x>=0 && point.x<=1 && point.y>=0 && point.y<=1 && point.z>=0 && point.z<=1);
            if (contains) ans = std::min(ans, glm::length(point-origin));
        }

        {
            glm::mat3 mat(__shape[1], __shape[3], - direction);
            glm::vec3 point = glm::inverse(mat)*origin;
            contains = (point.x>=0 && point.x<=1 && point.y>=0 && point.y<=1 && point.z>=0);
            if (contains) ans = std::min(ans, glm::length(point-origin));
        }

        {
            glm::mat3 mat(__shape[2], __shape[3], - direction);
            glm::vec3 point = glm::inverse(mat)*origin;
            contains = (point.x>=0 && point.x<=1 && point.y>=0 && point.y<=1 && point.z>=0);
            if (contains) ans = std::min(ans, glm::length(point-origin));
        }

        origin -= __shape[1]+__shape[2]+__shape[3];

        {
            glm::mat3 mat(-__shape[1], -__shape[2], - direction);
            glm::vec3 point = glm::inverse(mat)*origin;
            contains = (point.x>=0 && point.x<=1 && point.y>=0 && point.y<=1 && point.z>=0);
            if (contains) ans = std::min(ans, glm::length(point-origin));
        }

        {
            glm::mat3 mat(-__shape[1], -__shape[3], - direction);
            glm::vec3 point = glm::inverse(mat)*origin;
            contains = (point.x>=0 && point.x<=1 && point.y>=0 && point.y<=1 && point.z>=0);
            if (contains) ans = std::min(ans, glm::length(point-origin));
        }

        {
            glm::mat3 mat(-__shape[2], -__shape[3], - direction);
            glm::vec3 point = glm::inverse(mat)*origin;
            contains = (point.x>=0 && point.x<=1 && point.y>=0 && point.y<=1 && point.z>=0);
            if (contains) ans = std::min(ans, glm::length(point-origin));
        }
        return ans;
    }
    void HitboxNode::update() {
        if (tree) tree->update(this);
    }
    void HitboxNode::setShape(const glm::mat4x3& shape) {
        __shape = shape;
        glm::vec3 poses[8] = {shape[0], shape[0]+shape[1], shape[0]+shape[2], shape[0]+shape[1]+shape[2],
                            shape[0] + shape[3], shape[0]+shape[1] + shape[3], shape[0]+shape[2] + shape[3], shape[0]+shape[1]+shape[2] + shape[3]};
        x = y = z = {std::numeric_limits<float>::max(), std::numeric_limits<float>::min()};
        for (int i = 0; i<8; i++) {
            x.x = std::min(x.x, poses[i].x);
            x.y = std::max(x.y, poses[i].x);

            y.x = std::min(y.x, poses[i].y);
            y.y = std::max(y.y, poses[i].y);

            z.x = std::min(z.x, poses[i].z);
            z.y = std::max(z.y, poses[i].z);
        }
    }
    glm::mat4x3 HitboxNode::getShape() const {
        return __shape;
    }

    float HitboxNode::operator-(const HitboxNode& node) const {
        glm::vec3 offset;
        offset.x = std::max(x.y, node.x.y)-std::min(x.x, node.x.x) - (x.y-x.x);
        offset.y = std::max(y.y, node.y.y)-std::min(y.x, node.y.x) - (y.y-y.x);
        offset.z = std::max(z.y, node.z.y)-std::min(z.x, node.z.x) - (z.y-z.x);
        return glm::length(offset);
    }
    HitboxNode HitboxNode::operator+(const HitboxNode& node) const {
        HitboxNode new_node(tree);
        new_node.x.x = std::min(x.x, node.x.x);
        new_node.x.y = std::max(x.y, node.x.y);

        new_node.y.x = std::min(y.x, node.y.x);
        new_node.y.y = std::max(y.y, node.y.y);

        new_node.z.x = std::min(z.x, node.z.x);
        new_node.z.y = std::max(z.y, node.z.y);

        new_node.__shape = {{new_node.x.x, new_node.y.x, new_node.z.x}, {new_node.x.y-new_node.x.x, 0, 0}, 
                            {0, new_node.y.y-new_node.y.x, 0}, {0, 0, new_node.z.y - new_node.z.x}};
        // std::cout << "Merge: " << *this << " " << node <<  " " << new_node << std::endl;
        return new_node;
    }

    HitboxTree::HitboxTree(): __root(0) {}
    HitboxTree::~HitboxTree() {}

    std::vector<HitboxNode*> HitboxTree::get(const glm::vec3& position) const {
        std::vector<HitboxNode*> ans;
        __get(ans, __root, position);
        return ans;
    }
    HitboxNode* HitboxTree::get(const glm::vec3& position, const glm::vec3& direction) const {
        return __get(__root, position, direction).first;
    }
    void HitboxTree::insert(HitboxNode* model) {
        model->tree = this;
        __insert(__root, model);
        print();
    }
    void HitboxTree::print() const {
        __print(__root);
    }
    void HitboxTree::remove(HitboxNode* model) {
        __remove(__root, model);
    }
    bool HitboxTree::__remove(HitboxNode*& root, HitboxNode* node) {
        if (!root) return false;
        else if (!root->height) {
            if (root->getShape() == node->getShape()) {
                root = 0;
                return true;
            }
            return false;
        }
        else {
            if ((*root->right- *node)<0.001) {
                bool isRemove = __remove(root->right, node);
                if (!root->right) {
                    HitboxNode* tmp = root->left;
                    tmp->parent = root->parent;
                    root->left = 0;
                    delete root;
                    root = tmp;
                    return true;
                }
                else if (isRemove) {
                    HitboxNode tmp = *root->left + *root->right;
                    tmp.left = root->left;
                    tmp.right = root->right;
                    tmp.parent = root->parent;
                    tmp.height = std::max(root->left->height, root->right->height)+1;
                    *root = tmp;
                    tmp.right = tmp.left = 0;
                    
                    if (root->left->height - root->right->height >= 2) {
                        if (root->left->right && root->left->right->height>root->left->left->height) __rotateLeft(root->left);
                        __rotateRight(root);
                    }
                    root->height = std::max(root->left->height, root->right->height)+1;
                    return true;
                }
            }
            if ((*root->left - *node) < 0.001) {
                bool isRemove = __remove(root->left, node);
                if (!root->left) {
                    HitboxNode* tmp = root->right;
                    tmp->parent = root->parent;
                    root->right = 0;
                    delete root;
                    root = tmp;
                    return true;
                }
                else if (isRemove) {
                    HitboxNode tmp = *root->right + *root->left;
                    tmp.left = root->left;
                    tmp.right = root->right;
                    tmp.parent = root->parent;
                    tmp.height = std::max(root->left->height, root->right->height)+1;
                    *root = tmp;
                    tmp.right = tmp.left = 0;

                    if (root->right->height - root->left->height >= 2) {
                        if (root->right->left && root->right->left->height > root->right->right->height) __rotateRight(root->right);
                        __rotateLeft(root);
                    }
                    root->height = std::max(root->left->height, root->right->height)+1;
                    return true;
                }
            }
            return false;
        }
    }
    void HitboxTree::__insert(HitboxNode*& root, HitboxNode* node) {
        if (!root) root = node;
        else if (!root->height) {
            HitboxNode* tmp = new HitboxNode(this);
            *tmp = *root + *node;
            tmp->left = node;
            node->parent = tmp;
            tmp->right = root;
            tmp->parent = root->parent;
            root->parent = tmp;
            tmp->height = 1;
            root = tmp;
        }
        else if (((*root->right - *node)<(*root->left-*node))) {
            __insert(root->right, node);
            HitboxNode tmp = *root->right + *root->left;
            tmp.left = root->left;
            tmp.right = root->right;
            tmp.parent = root->parent;
            tmp.height = std::max(tmp.left->height, tmp.right->height)+1;
            *root = tmp;
            tmp.left = tmp.right = 0;
            if (root->right->height - root->left->height >= 2) {
                if (root->right->left && root->right->left->height>root->right->right->height) __rotateRight(root->right);
                __rotateLeft(root);
            }
            root->height = std::max(root->left->height, root->right->height)+1;
        }
        else {
            __insert(root->left, node);
            HitboxNode tmp = *root->right + *root->left;
            tmp.left = root->left;
            tmp.right = root->right;
            tmp.parent = root->parent;
            tmp.height = std::max(tmp.left->height, tmp.right->height)+1;
            *root = tmp;
            tmp.left = tmp.right = 0;
            if (root->left->height - root->right->height >= 2) {
                if (root->left->right && root->left->right->height>root->left->left->height) __rotateLeft(root->left);
                __rotateRight(root);
            }
            root->height = std::max(root->left->height, root->right->height)+1;
        }
    }
    void HitboxTree::__rotateLeft(HitboxNode*& root) {
        HitboxNode* tmp = root->right;
        root->right = tmp->left;
        root->right->parent = root;
        {
            HitboxNode merge = *root->right + *root->left;
            merge.left = root->left;
            merge.right = root->right;
            merge.parent = root->parent;
            merge.height = std::max(root->left->height, root->right->height) + 1;
            *root = merge;
            merge.left = merge.right = 0;
        }
        tmp->left = root;
        tmp->parent = root->parent;
        root->parent = tmp;
        root = tmp;
        {
            HitboxNode merge = *root->right + *root->left;
            merge.left = root->left;
            merge.parent = root->parent;
            merge.right = root->right;
            merge.height = std::max(root->left->height, root->right->height) + 1;
            *root = merge;
            merge.left = merge.right = 0;
        }
    }
    void HitboxTree::__rotateRight(HitboxNode*& root) {
        HitboxNode* tmp = root->left;
        root->left = tmp->right;
        tmp->right->parent = root;
        {
            HitboxNode merge = *root->right + *root->left;
            merge.left = root->left;
            merge.right = root->right;
            merge.parent = root->parent;
            merge.height = std::max(root->left->height, root->right->height) + 1;
            *root = merge;
            merge.left = merge.right = 0;
        }
        tmp->right = root;
        tmp->parent = root->parent;
        root->parent = tmp;
        root = tmp;
        {
            HitboxNode merge = *root->right + *root->left;
            merge.left = root->left;
            merge.right = root->right;
            merge.parent = root->parent;
            merge.height = std::max(root->left->height, root->right->height) + 1;
            *root = merge;
            merge.left = merge.right = 0;
        }
    }
    void HitboxTree::update(HitboxNode* node) {
        if (!node || !node->parent) return ;
        if ((*node->parent - *node)==0) return ;
        else {
            HitboxNode* root = node->parent;
            while (root) {
                HitboxNode tmp = *root->right + *root->left;
                tmp.parent = root->parent;
                tmp.left = root->left;
                tmp.right = root->right;
                tmp.height = std::max(tmp.left->height, tmp.right->height)+1;
                *root = tmp;
                tmp.left = tmp.right = 0;
                root = root->parent;
            }
        }
    }
    void HitboxTree::__print(HitboxNode* root) const {
        if (!root) return ;
        for (int i = 0; i<root->height; i++) std::cout << ' ';
        std::cout << *root << std::endl;
        __print(root->left);
        __print(root->right);
    }
    void HitboxTree::__get(std::vector<HitboxNode*>& ans, HitboxNode* root, const glm::vec3& position) const {
        if (!root) return ;
        if (root->left->contains(position)) {
            if (!root->left->height) ans.push_back(root->left);
            else __get(ans, root->left, position);
        }
        if (root->right->contains(position)) {
            if (!root->right->height) ans.push_back(root->right);
            else __get(ans, root->right, position);
        }
    }
    std::pair<HitboxNode*, float> HitboxTree::__get(HitboxNode* root, const glm::vec3& position, const glm::vec3& dir) const {
        if (!root) return {0,0};
        std::pair<HitboxNode*, float> ans = {0,std::numeric_limits<float>::max()};
        if (float distance = root->left->isCollistion(position, dir); distance<=glm::length(dir)) {
            std::pair<HitboxNode*, float> subans;
            if (!root->left->height) subans = {root->left, distance};
            else subans = __get(root->left, position, dir);
            if (subans.second<ans.second) ans = subans;
        }
        if (float distance = root->right->isCollistion(position, dir); distance<=glm::length(dir)) {
            std::pair<HitboxNode*, float> subans;
            if (!root->right->height) subans = {root->right, distance};
            else subans = __get(root->right, position, dir);
            if (subans.second<ans.second) ans = subans;
        }
        return ans;
    }
}