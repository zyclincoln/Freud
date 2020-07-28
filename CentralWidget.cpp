#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QDebug>
#include <iostream>

#include "CentralWidget.h"
#include "BallBoundary.h"

using namespace std;

CentralWidget::CentralWidget(QWidget *parent) : QWidget(parent)
{
    parameter_ = shared_ptr<Parameter>(new Parameter());
    simulator_ = new Simulator(300, 300, parameter_);
    drawer_ = new Drawer(simulator_->get_density_field(), this);

    start_ = new QPushButton("start");
    QGroupBox* control_box = new QGroupBox("控制 (control)", this);
    QHBoxLayout* control_box_layout = new QHBoxLayout();
    control_box_layout->addWidget(start_);
    control_box->setLayout(control_box_layout);

    use_upwind_ = new QCheckBox("上风积分");
    use_runge_kutta_ = new QCheckBox("龙格库塔积分");
    use_upwind_->setDisabled(true);
    
    QGroupBox* integrate_box = new QGroupBox("积分方法 (integrate)", this);
    QVBoxLayout* integrate_box_layout = new QVBoxLayout();
    integrate_box_layout->addWidget(use_upwind_);
    integrate_box_layout->addWidget(use_runge_kutta_);
    integrate_box->setLayout(integrate_box_layout);

    color_picker_ = new ColorPicker();

    QGroupBox* color_box = new QGroupBox("颜色 (color)",this);
    QVBoxLayout* color_layout = new QVBoxLayout();
    color_layout->addWidget(color_picker_);
    color_box->setLayout(color_layout);

    time_step_slider_ = new QSlider(Qt::Horizontal);
    time_step_slider_->setTickInterval(1);
    time_step_slider_->setMinimum(1);
    time_step_slider_->setMaximum(100);
    // multiplier 100
    time_step_slider_->setValue(parameter_->dt*100);

    visc_slider_ = new QSlider(Qt::Horizontal);
    visc_slider_->setTickInterval(10);
    visc_slider_->setMinimum(10);
    visc_slider_->setMaximum(1000);
    // multiplier 10000000
    visc_slider_->setValue(parameter_->visc*10000000);

    diffuse_slider_ = new QSlider(Qt::Horizontal);
    diffuse_slider_->setTickInterval(10);
    diffuse_slider_->setMinimum(10);
    diffuse_slider_->setMaximum(1000);
    // multiplier 10000000
    diffuse_slider_->setValue(parameter_->diffuse*10000000);

    dissipation_slider_ = new QSlider(Qt::Horizontal);
    dissipation_slider_->setTickInterval(10);
    dissipation_slider_->setMinimum(10);
    dissipation_slider_->setMaximum(1000);
    // multiplier 100000
    dissipation_slider_->setValue(parameter_->dissipation*100000);

    vorticity_slider_ = new QSlider(Qt::Horizontal);
    vorticity_slider_->setTickInterval(10);
    vorticity_slider_->setMinimum(10);
    vorticity_slider_->setMaximum(1000);
    // multiplier 10000
    vorticity_slider_->setValue(parameter_->vorticity*10000);

    QGroupBox* parameter_box = new QGroupBox("参数 (parameter)", this);
    QGridLayout* parameter_layout = new QGridLayout();
    parameter_layout->addWidget(new QLabel("时间步长:"), 0, 0);
    parameter_layout->addWidget(time_step_slider_, 0, 1, 1, 3);
    parameter_layout->addWidget(new QLabel("粘度系数:"), 1, 0);
    parameter_layout->addWidget(visc_slider_, 1, 1, 1, 3);
    parameter_layout->addWidget(new QLabel("扩散系数:"), 2, 0);
    parameter_layout->addWidget(diffuse_slider_, 2, 1, 1, 3);
    parameter_layout->addWidget(new QLabel("消散系数:"), 3, 0);
    parameter_layout->addWidget(dissipation_slider_, 3, 1, 1, 3);
    parameter_layout->addWidget(new QLabel("漩涡增强:"), 4, 0);
    parameter_layout->addWidget(vorticity_slider_, 4, 1, 1, 3);
    parameter_box->setLayout(parameter_layout);
    parameter_box->setMinimumWidth(600);

    QVBoxLayout* control_layout = new QVBoxLayout();
    control_layout->addWidget(control_box);
    control_layout->addWidget(integrate_box);
    control_layout->addWidget(color_box);
    control_layout->addWidget(parameter_box);

    QHBoxLayout* main_layout = new QHBoxLayout(this);
    main_layout->addWidget(drawer_);
    main_layout->addLayout(control_layout);

    this->setLayout(main_layout);

    connect(start_,     &QPushButton::clicked,      this,       &CentralWidget::start);
    connect(simulator_, &Simulator::update_finish,  drawer_,    &Drawer::redraw);
    connect(drawer_,    &Drawer::draw_finish,       this,       &CentralWidget::next);
    connect(drawer_,    &Drawer::add_boundary,      this,       &CentralWidget::add_boundary);
}

void CentralWidget::start(){
    simulator_->start();
}

void CentralWidget::add_boundary(int x, int y, double dx, double dy){
    conditions.push_back(make_shared<BallBoundary>(x, y, dx, dy, color_picker_->getColor()));
}

void CentralWidget::next(){
    // update parameter
    parameter_->dt = time_step_slider_->value()*1.0/100;
    parameter_->visc = visc_slider_->value()*1.0/10000000;
    parameter_->diffuse = diffuse_slider_->value()*1.0/10000000;
    parameter_->vorticity = vorticity_slider_->value()*1.0/10000;
    parameter_->dissipation = dissipation_slider_->value()*1.0/100000;
    parameter_->use_rk4 = use_runge_kutta_->isChecked();
    // update condition
    while(conditions.size() > 0){
        conditions.back()->apply_density(simulator_->get_d_source());
        conditions.back()->apply_velocity(simulator_->get_v_source());
        conditions.pop_back();
    }
    parameter_->draw_finish = true;
}
