#include <QDebug>

#include "Simulator.h"
#include "Operator.hpp"

using namespace std;

Simulator::Simulator(int width, int height, shared_ptr<Parameter> parameter)
    : v_(width, height), d_(width, height), parameter_(parameter),
      v_source_(width, height), d_source_(width, height)
{
    for(int y = 100; y < 110; ++y){
        for(int x = 60; x < 70; ++x){
            d_source_(x, y, 0) = 0.8;
            v_source_(x, y, 1) = -0.2;
        }
    }
}

void Simulator::run(){
    while(!parameter_->should_close){
        while(!parameter_->draw_finish){
            qDebug() << "waiting";
        }

        // v step
        add_source(v_, v_source_, parameter_->dt);
        vorticity_confinement(v_, parameter_->vorticity);
        diffuse(v_, parameter_->visc, parameter_->dt);
        Field<double, 2> temp_v = v_;
        advect(v_, temp_v, temp_v, parameter_->dt);
        project(v_);

        // d step
        add_source(d_, d_source_, parameter_->dt);
        diffuse(d_, parameter_->diffuse, parameter_->dt);
        Field<double, 1> temp_d = d_;
        advect(d_, temp_d, v_, parameter_->dt);
        dissipate(d_, parameter_->dissipation);

        parameter_->draw_finish = false;
        qDebug() << "frame: " << frame_count_++;
        emit update_finish();
    }
}
