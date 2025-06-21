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
                        getRectangleIndices() const,
                        getListIndicesInput() const,
                        getMarginBlockIndices() const,
                        getImageBlockIndices() const;
        private:
            GLuint      __marginBlockIndices,
                        __imageBlockIndices,
                        __listIndicesInput,
                        __rectangleIndices;
            GLuint      __blockSet,
                        __chunkSet;
        };
}
#endif