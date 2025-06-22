#ifndef CHUNK_H
#define CHUNK_H
#include "Block.h"
#include "Ray.h"
#include <bitset>
#include <unordered_map>
#include <unordered_set>
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
                            
            const BlockCatogary::Catogary   &at(const int& x, const int& y, const int& z) const;
            void            set(const int& x, const int& y, const int& z, const BlockCatogary::Catogary& type);
            BlockCatogary::Catogary& getBlocks(const int& x, const int& y, const int& z);
            friend class World;
        protected:
            virtual void    glDraw(const glm::vec3& position, const glm::vec3& dir) const;
        private:
            unsigned int                    __bitOn;
            glm::vec3                       __position;
            std::bitset<16>                 __bits[16][16];
            std::map<BlockCatogary::Catogary, std::vector<glm::vec4>> __list;
            std::vector<std::vector<std::vector<BlockCatogary::Catogary>>>  __blocks;
            int                             __horizontalPlane[16];

            void __enableBit(const int& x, const int& y, const int& z);
            void __disableBit(const int& x, const int& y, const int& z);
    };
};
#endif