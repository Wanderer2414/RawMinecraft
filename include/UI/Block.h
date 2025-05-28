#ifndef BLOCK_H
#define BLOCK_H
#include "Controller3D.h"
#include "Ray.h"
#include "Rect3D.h"
namespace MyCraft {
    class BlockCatogary {
        public:
            enum Catogary {
                Air, Dirt
            };
            BlockCatogary();
            ~BlockCatogary();
        
            GLuint getBlock(const int& index) const;
            GLuint BlockTexture;
            static BlockCatogary* Default;
        private:
            vector<GLuint>      pPtr;
        };
        class Chunk;
        class Block {
        public:
            Block();
            ~Block();
            BlockCatogary::Catogary type;
            char                getHoverPlane() const;
            virtual void        setPosition(const float& x, const float& y, const float& z),
                                setPosition(const glm::vec3& position);
            virtual void        setHoverPlane(const char& index);
            friend class Chunk;
        protected:
            virtual void        glDraw() const;
        private:
            char                pHoverPlane;
            glm::vec3           pPosition;
    };
}
#endif