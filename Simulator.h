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

    Field<double, 1>& get_density_field(){
        return d_;
    }
    void run();

signals:
    void update_finish();

private:
    int frame_count_ = 0;

    Field<double, 2> v_;
    Field<double, 1> d_;
    std::shared_ptr<Parameter> parameter_;

    Field<double, 2> v_source_;
    Field<double, 1> d_source_;
};

#endif // SIMULATOR_H
