#include <QDebug>
#include <iostream>
#include <QTime>

#include "Simulator.h"
#include "Operator.hpp"

using namespace std;

Simulator::Simulator(int width, int height, shared_ptr<Parameter> parameter)
    : v_(width, height),
      d_(vector<Field<double, 1>>(3, Field<double, 1>(width, height))),
      parameter_(parameter),
      v_source_(width, height),
      d_source_(vector<Field<double, 1>>(3, Field<double, 1>(width, height)))
{
//    for(int y = 100; y < 110; ++y){
//        for(int x = 60; x < 70; ++x){
//            v_source_(x, y, 1) = -0.2;
//            d_source_[0](x, y, 0) = 0.8;
//        }
//    }
}

void Simulator::run(){
    while(!parameter_->should_close){
        while(!parameter_->draw_finish){

        }
        QTime start = QTime::currentTime();
        // v step
        add_source(v_, v_source_, parameter_->dt);
        v_source_.clear();
        QTime t1 = QTime::currentTime();
        qDebug() << "add source cost: " << (t1.second()-start.second())*1000 + (t1.msec() - start.msec());

        vorticity_confinement(v_, parameter_->vorticity);
        QTime t2 = QTime::currentTime();
        qDebug() << "vorticity cost: " << (t2.second()-t1.second())*1000 + (t2.msec() - t1.msec());

        diffuse(v_, parameter_->visc, parameter_->dt);
        QTime t3 = QTime::currentTime();
        qDebug() << "diffuse cost: " << (t3.second()-t2.second())*1000 + (t3.msec() - t2.msec());

        Field<double, 2> temp_v = v_;
        advect(v_, temp_v, temp_v, parameter_->dt);
        QTime t4 = QTime::currentTime();
        qDebug() << "advect cost: " << (t4.second()-t3.second())*1000 + (t4.msec() - t3.msec());

        project(v_);
        QTime t5 = QTime::currentTime();
        qDebug() << "project cost: " << (t5.second()-t4.second())*1000 + (t5.msec() - t4.msec());

        // d step
        for(size_t i = 0; i < d_.size(); ++i){
            add_source(d_[i], d_source_[i], parameter_->dt);
            d_source_[i].clear();

            diffuse(d_[i], parameter_->diffuse, parameter_->dt);
            Field<double, 1> temp_d = d_[i];
            advect(d_[i], temp_d, v_, parameter_->dt);
            dissipate(d_[i], parameter_->dissipation);
        }
        QTime end = QTime::currentTime();
        qDebug() << "computation cost: " << (end.second()-start.second())*1000 + (end.msec() - start.msec());
        parameter_->draw_finish = false;
        qDebug() << "frame: " << frame_count_++;
        emit update_finish();
    }
}
