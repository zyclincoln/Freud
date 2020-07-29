#ifndef BOUNDARYCONDITION_H
#define BOUNDARYCONDITION_H
#include <QColor>
#include <QPoint>
#include "Field.h"

class BoundaryCondition
{
public:
    BoundaryCondition(const QPoint last,
                      const QPoint cur,
                      const QColor& color,
                      const int w, const int h):
        last_(last), cur_(cur), color_(color), w_(w), h_(h){

    }

    virtual void apply_density(std::vector<Field<double, 1>>& d) {
        Q_UNUSED(d)
    };
    virtual void apply_velocity(Field<double, 2>& v) {
        Q_UNUSED(v)
    };

protected:
    QPoint last_, cur_;
    int w_, h_;
    QColor color_;
};

#endif // BOUNDARYCONDITION_H
