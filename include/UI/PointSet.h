#ifndef POINTSET_H
#define POINTSET_H

#include <Global.h>
namespace MyBase3D {
    class PointSet {
        public:
            static PointSet& getInstance();
            static void close();
            GLuint      getBlockSet() const,
                        getChunkSet() const,
                        getRectangleIndices() const,
                        getMarginBlockIndices() const,
                        getImageBlockIndices() const;
        private:
            PointSet();
            ~PointSet();
            GLuint      __marginBlockIndices,
                        __imageBlockIndices,
                        __rectangleIndices;
            GLuint      __blockSet;
            static PointSet* Default;
        };
}
#endif