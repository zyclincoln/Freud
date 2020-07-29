#ifndef BALLBOUNDARY_H
#define BALLBOUNDARY_H
#include "BoundaryCondition.h"

class BallBoundary: public BoundaryCondition
{
public:
    BallBoundary(const QPoint last,
                 const QPoint cur, 
                 const QColor& color,
                 const int w, const int h):
        BoundaryCondition(last, cur, color, w, h){ }
    virtual void apply_density(std::vector<Field<double, 1> > &d) override;
    virtual void apply_velocity(Field<double, 2> &v) override;
private:
    int r_ = 8;
};

#endif // BALLBOUNDARY_H
