#ifndef POINTSET_H
#define POINTSET_H

#include <Global.h>
namespace MyBase3D {
    class PointSet {
        public:
            PointSet();
            ~PointSet();
        
            static PointSet* Default;
        
            GLuint BlockSet;
            GLuint ChunkSet;
            GLuint MarginSet;
        private:
        };
}
#endif