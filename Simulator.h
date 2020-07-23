#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <QObject>
#include <memory>
#include <QThread>

#include "Parameter.h"
#include "Field.h"

class Simulator : public QThread
{
    Q_OBJECT
public:
    explicit Simulator(int width, int height, std::shared_ptr<Parameter> parameter);

    std::vector<Field<double, 1>>& get_density_field(){
        return d_;
    }
    void run();

    std::vector<Field<double, 1>>& get_d_source(){
        return d_source_;
    }
    Field<double, 2>& get_v_source(){
        return v_source_;
    }

signals:
    void update_finish();

private:
    int frame_count_ = 0;

    Field<double, 2> v_;
    std::vector<Field<double, 1>> d_;

    std::shared_ptr<Parameter> parameter_;

    Field<double, 2> v_source_;
    std::vector<Field<double, 1>> d_source_;
};

#endif // SIMULATOR_H
