#ifndef CENTRALWIDGET_H
#define CENTRALWIDGET_H

#include <QWidget>
#include <QCheckBox>
#include <QPalette>
#include <QPushButton>
#include <QSlider>
#include <QThread>
#include <memory>
#include <queue>

#include "Simulator.h"
#include "Parameter.h"
#include "Drawer.h"
#include "ColorPicker.h"
#include "BoundaryCondition.h"

class CentralWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CentralWidget(QWidget *parent = nullptr);
    ~CentralWidget(){
        simulator_->quit();
    }

public slots:
    void start();
    void next();
    void add_boundary(QPoint last, QPoint cur, int w, int h);
signals:


private:
    std::vector<std::shared_ptr<BoundaryCondition>> conditions;

    Simulator* simulator_;
    Drawer* drawer_;
    std::shared_ptr<Parameter> parameter_;

    // simulate control
    QPushButton* start_;

    // choose addition function
    QCheckBox* use_upwind_;
    QCheckBox* use_runge_kutta_;

    // choose color
    ColorPicker* color_picker_;

    // setup parameter
    QSlider* time_step_slider_;
    QSlider* visc_slider_;
    QSlider* diffuse_slider_;
    QSlider* dissipation_slider_;
    QSlider* vorticity_slider_;
};

#endif // CENTRALWIDGET_H
