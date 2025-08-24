#ifndef POINTSET_H
#define POINTSET_H

#include <Global.h>
namespace MyBase3D {
    class PointSet {
        public:
            static PointSet& getInstance();
            static void close();
            GLuint      getBlockSet() const,
                        getBlockUVS() const,
                        getChunkSet() const,
                        getRectangle2DSet() const,
                        getRectangle3DSet() const,
                        getRectangle2DUV() const,
                        getRectangle3DIndices() const,
                        getRectangleIndices() const,
                        getMarginBlockIndices() const,
                        getImageBlockIndices() const;
        private:
            PointSet();
            ~PointSet();
            GLuint      __marginBlockIndices,
                        __imageBlockIndices,
                        __rectangle2DSet, __rectangle2DUV,
                        __rectangle3DSet, __rectangle3DIndice,
                        __rectangleIndices;
            GLuint      __blockSet, __blockUVS;
            static PointSet* Default;
        };
}
#endif