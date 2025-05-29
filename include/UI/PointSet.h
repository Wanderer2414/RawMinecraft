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
                        getMarginSet() const;
        private:
            GLuint      __blockSet,
                        __chunkSet,
                        __marginSet;
        };
}
#endif