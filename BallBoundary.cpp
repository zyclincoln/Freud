#include <algorithm>
#include <QDebug>

#include "BallBoundary.h"
using namespace std;

void BallBoundary::apply_density(std::vector<Field<double, 1> > &d) {
    double rgb[3] = {color_.redF(), color_.greenF(), color_.blueF()};
    int cur_x = cur_.x() * 1.0 / w_ * d[0].get_width(),
        cur_y = cur_.y() * 1.0 / h_ * d[0].get_width(),
        las_x = last_.x() * 1.0 / w_ * d[0].get_height(),
        las_y = last_.y() * 1.0 / w_ * d[0].get_height();

    int minX = max(1, min(las_x, cur_x - r_));
    int maxX = min(d[0].get_width() - 2, max(las_x, cur_x + r_));
    int minY = max(1, min(las_y, cur_y - r_));
    int maxY = min(d[0].get_height() - 2, max(las_y, cur_y + r_));

    qDebug() << last_ << " - " << cur_;
    qDebug() << minX << " " << maxX << " - " << minY << " " << maxY;
    for(int i = 0; i < 3; ++i){
        for(int y = minY; y <= maxY; ++y){
            for(int x = minX; x <= maxX; ++x){
                double r = (x*1.0 - cur_x) * (x - cur_x) + (y - cur_y) * (y - cur_y);
                if (r <= r_*1.0 * r_) {
                    d[i](x, y, 0) += rgb[i] * (1-r/(r_*r_));
                }
                else {

                }
            }
        }
    }
}

void BallBoundary::apply_velocity(Field<double, 2> &v) {
    int cur_x = cur_.x() * 1.0 / w_ * v.get_width(),
        cur_y = cur_.y() * 1.0 / h_ * v.get_width(),
        las_x = last_.x() * 1.0 / w_ * v.get_height(),
        las_y = last_.y() * 1.0 / w_ * v.get_height();

    int minX = max(1, min(las_x - r_, cur_x - r_));
    int maxX = min(v.get_width() - 2, max(las_x + r_, cur_x + r_));
    int minY = max(1, min(las_y - r_, cur_y - r_));
    int maxY = min(v.get_height() - 2, max(las_y + r_, cur_y + r_));

    for (int y = minY; y <= maxY; ++y) {
        for (int x = minX; x <= maxX; ++x) {
            if ((x - cur_x) * (x - cur_x) + (y - cur_y) * (y - cur_y) <= r_ * r_) {
                // direction speed
                v(x, y, 0) += (last_.x()*1.0 - cur_.x())/w_ *10;
                v(x, y, 1) += (last_.y()*1.0 - cur_.y())/h_ *10;

                // ejection speed
                double dx = (x*1.0 - cur_x) / v.get_width() * 10;
                double dy = (y*1.0 - cur_y) / v.get_height() * 10;
                v(x, y, 0) += dx;
                v(x, y, 1) += dy;
            }
        }
    }
}
