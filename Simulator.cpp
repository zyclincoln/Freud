#include <QDebug>
#include <iostream>
#include <QTime>

#include "Simulator.h"
#include "Operator.hpp"

using namespace std;
using namespace Eigen;

Simulator::Simulator(int width, int height, shared_ptr<Parameter> parameter)
    : v_(width, height), w_(width), h_(height),
      d_(vector<Field<double, 1>>(3, Field<double, 1>(width, height))),
      parameter_(parameter),
      v_source_(width, height),
      d_source_(vector<Field<double, 1>>(3, Field<double, 1>(width, height))){
}

void Simulator::run(){

    // construct sparse matrix A_
    double scale = parameter_->dt * parameter_->diffuse / (w_ * 1.0 * h_);

    std::vector<Triplet<double>> triplets;
    for (int y = 0; y < h_; ++y) {
        for (int x = 0; x < w_; ++x) {
            int pos = y * w_ + x;
            int accu = 0;
            if (y > 0) {
                triplets.emplace_back(pos, pos - w_, -scale);
                accu++;
            }
            if (y < h_ - 1) {
                triplets.emplace_back(pos, pos + w_, -scale);
                accu++;
            }
            if (x > 0) {
                triplets.emplace_back(pos, pos - 1, -scale);
                accu++;
            }
            if (x < w_ - 1) {
                triplets.emplace_back(pos, pos + 1, -scale);
                accu++;
            }
            triplets.emplace_back(pos, pos, accu * scale + 1);
        }
    }
    A_.resize(w_*h_, w_*h_);
    A_.setFromTriplets(triplets.begin(), triplets.end());
    cg_.compute(A_);

    // enter solve loop
    while(!parameter_->should_close){
        QTime before_wait = QTime::currentTime();
        while(!parameter_->draw_finish){
        }
        QTime after_wait = QTime::currentTime();
        // v step
        add_source(v_, v_source_, parameter_->dt);
        v_source_.clear();

        vorticity_confinement(v_, parameter_->vorticity);
        // for smoke simulation, I remove the viscosity term
        //diffuse(v_, parameter_->visc, parameter_->dt);

        Field<double, 2> temp_v = v_;
        if (parameter_->use_rk4) {
            advect_rk4(v_, temp_v, temp_v, parameter_->dt);
        }
        else {
            advect(v_, temp_v, temp_v, parameter_->dt);
        }
        project(v_);

        // d step
        for(size_t i = 0; i < d_.size(); ++i){
            add_source(d_[i], d_source_[i], parameter_->dt);
            d_source_[i].clear();

            diffuse(d_[i], cg_);
            Field<double, 1> temp_d = d_[i];
            if (parameter_->use_rk4) {
                advect_rk4(d_[i], temp_d, v_, parameter_->dt);
            }
            else {
                advect(d_[i], temp_d, v_, parameter_->dt);
            }
            dissipate(d_[i], parameter_->dissipation);
        }
        QTime end = QTime::currentTime();
        parameter_->draw_finish = false;
        qDebug() << "frame: " << frame_count_++;
        qDebug() << "waiting: " << (after_wait.second() - before_wait.second()) * 1000 + after_wait.msec() - before_wait.msec();
        qDebug() << "calculation: " << (end.second() - after_wait.second()) * 1000 + end.msec() - after_wait.msec();
        emit update_finish();
    }
}
