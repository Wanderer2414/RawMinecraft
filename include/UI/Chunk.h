#ifndef CHUNK_H
#define CHUNK_H
#include "Block.h"
#include "Ray.h"
namespace MyCraft {
    class World;
    class Chunk {
        public:
            Chunk();
            ~Chunk();
            bool            contains(const glm::vec3& point) const;
            virtual bool    setHover(const MyBase3D::Ray3f& ray);

            virtual void    setPosition(const int& x, const int& y, const int& z),
                            setPosition(const glm::vec3& position);
                            
            Block           &at(const int& x, const int& y, const int& z);
            BlockCatogary::Catogary& getBlocks(const int& x, const int& y, const int& z);
            friend class World;
        protected:
            virtual void    glDraw() const;
        private:
            glm::vec3                    __position;
            Block                        __blocks[16][16][16];
    };
};
#endif