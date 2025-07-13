#ifndef BLOCK_H
#define BLOCK_H
#include "Global.h"
namespace MyCraft {
    class BlockCatogary {
        public:
            enum Catogary: unsigned char {
                Air, Dirt, Grass
            };
            static BlockCatogary& getInstance();
            static void close();
        
            GLuint      getBlock(const int& index) const,
                        getTexCoord() const;
        private:
            BlockCatogary();
            ~BlockCatogary();
            static BlockCatogary* Default;
            std::vector<GLuint> __ptr;
            GLuint              __blockTexture;
        };
}
#endif