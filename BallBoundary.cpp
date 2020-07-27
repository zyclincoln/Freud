#include <algorithm>
#include <QDebug>

#include "BallBoundary.h"
using namespace std;

void BallBoundary::apply_density(std::vector<Field<double, 1> > &d) {
    double rgb[3] = {color_.redF(), color_.greenF(), color_.blueF()};
    for(int i = 0; i < 3; ++i){
        for(int y = max(1, y_-r_); y <= min(d[i].get_height()-2, y_+r_); ++y){
            for(int x = max(1, x_-r_); x <= min(d[i].get_width()-2, x_+r_); ++x){
                d[i](x, y, 0) += rgb[i] * 0.1;
            }
        }
    }
}

void BallBoundary::apply_velocity(Field<double, 2> &v) {
    for(int y = max(1, y_-r_); y <= min(v.get_height()-2, y_+r_); ++y){
        for(int x = max(1, x_-r_); x <= min(v.get_width()-2, x_+r_); ++x){
            v(x, y, 0) += dx_ * 0.05;
            v(x, y, 1) += dy_ * 0.05;
        }
    }
}
