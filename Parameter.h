#ifndef PARAMETER_H
#define PARAMETER_H


class Parameter
{
public:
    Parameter(){}
    double dt = 0.05;
    double diffuse = 0.00001;
    double visc = 0.00001;
    double dissipation = 0.005;
    double vorticity = 0.02;
    bool should_close = false;
    bool draw_finish = true;
};

#endif // PARAMETER_H
