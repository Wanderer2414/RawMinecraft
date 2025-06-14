#ifndef POINTSET_H
#define POINTSET_H

#include <Global.h>
namespace MyBase3D {
    class PointSet {
        public:
            PointSet();
            ~PointSet();
        
            static PointSet* Default;

            GLuint      getBlockSet() const,
                        getChunkSet() const,
                        getMarginBlockIndices() const,
                        getImageBlockIndices() const;
        private:
            GLuint      __marginBlockIndices,
                        __imageBlockIndices;
            GLuint      __blockSet,
                        __chunkSet;
        };
}
#endif