#ifndef BALLBOUNDARY_H
#define BALLBOUNDARY_H
#include "BoundaryCondition.h"

class BallBoundary: public BoundaryCondition
{
public:
    BallBoundary(const int x, const int y, const double dx, const double dy, const QColor& color):
        BoundaryCondition(x, y, dx, dy, color){ }
    virtual void apply_density(std::vector<Field<double, 1> > &d) override;
    virtual void apply_velocity(Field<double, 2> &v) override;
private:
    int r_ = 5;
};

#endif // BALLBOUNDARY_H
