#ifndef PARAMETER_H
#define PARAMETER_H
#include <QObject>

class Parameter
{
public:
    double dt = 0.05;
    double diffuse = 0.00001;
    double visc = 0.00001;
    double dissipation = 0.01;
    double vorticity = 0.05;
    volatile bool use_rk4 = true;
    volatile bool should_close = false;
    volatile bool draw_finish = true;
    bool should_pause = true;
};

#endif // PARAMETER_H
