#ifndef BLOCK_H
#define BLOCK_H
#include "Global.h"
namespace MyCraft {
    class BlockCatogary {
        public:
            enum Catogary {
                Air, Dirt, Grass
            };

            BlockCatogary();
            ~BlockCatogary();
        
            GLuint      getBlock(const int& index) const,
                        getTexCoord() const;
            static BlockCatogary* Default;
        private:
            std::vector<GLuint> __ptr;
            GLuint              __blockTexture;
        };
        class Chunk;
        class Block {
        public:
            Block();
            ~Block();
            BlockCatogary::Catogary getType() const;
            char                getHoverPlane() const;
            virtual void        setPosition(const float& x, const float& y, const float& z),
                                setPosition(const glm::vec3& position);
            virtual void        setHoverPlane(const char& index);
            virtual void        setType(const BlockCatogary::Catogary& type);
            friend class Chunk;
        protected:
            virtual void        glDraw() const;
        private:
            char                        __hoverPlane;
            glm::vec3                   __position;
            BlockCatogary::Catogary     __type;
    };
}
#endif