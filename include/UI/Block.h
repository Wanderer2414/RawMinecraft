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
}
#endif