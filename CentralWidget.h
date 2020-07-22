#ifndef CENTRALWIDGET_H
#define CENTRALWIDGET_H

#include <QWidget>
#include <QCheckBox>
#include <QPalette>
#include <QPushButton>
#include <QSlider>
#include <QThread>
#include <memory>

#include "Simulator.h"
#include "Parameter.h"
#include "Drawer.h"

class CentralWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CentralWidget(QWidget *parent = nullptr);
    ~CentralWidget(){
        delete simulator_;
    }

public slots:
    void start();
    void next();
signals:


private:
    Simulator* simulator_;
    Drawer* drawer_;
    std::shared_ptr<Parameter> parameter_;

    // simulate control
    QPushButton* start_;

    // choose addition function
    QCheckBox* use_upwind_;
    QCheckBox* use_runge_kutta_;

    // choose color
    QPushButton* color_picker_;

    // setup parameter
    QSlider* time_step_slider_;
    QSlider* visc_slider_;
    QSlider* diffuse_slider_;
    QSlider* dissipation_slider_;
    QSlider* vorticity_slider_;
};

#endif // CENTRALWIDGET_H
