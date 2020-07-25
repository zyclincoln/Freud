#ifndef BOUNDARYCONDITION_H
#define BOUNDARYCONDITION_H
#include <QColor>
#include "Field.h"

class BoundaryCondition
{
public:
    BoundaryCondition(const int x, const int y,
                      const double dx, const double dy,
                      const QColor& color):
        x_(x), y_(y), dx_(dx), dy_(dy), color_(color){

    }

    virtual void apply_density(std::vector<Field<double, 1>>& d) {
        Q_UNUSED(d)
    };
    virtual void apply_velocity(Field<double, 2>& v) {
        Q_UNUSED(v)
    };

protected:
    int x_, y_;
    double dx_, dy_;
    QColor color_;
};

#endif // BOUNDARYCONDITION_H
