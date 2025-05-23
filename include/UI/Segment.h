#ifndef SEGMENT_H
#define SEGMENT_H
#include "Line.h"
class Segment: public Line {
public:
    Segment(const Vector3f& source, const Vector3f& destination);
    ~Segment();

    virtual bool    contains(const Vector3f& position) const override;
    virtual float   length() const;
private:
    
};
#endif