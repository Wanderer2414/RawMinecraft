#ifndef SEGMENT_H
#define SEGMENT_H
#include "Line.h"
namespace MyBase3D {
    class Segment: public Line {
    public:
        Segment(const glm::vec3& source, const glm::vec3& destination);
        ~Segment();
    
        virtual bool    contains(const glm::vec3& position) const override;
        virtual float   length() const;
    private:  
    };
};
#endif