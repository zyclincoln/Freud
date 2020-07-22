#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>

#include "CentralWidget.h"

using namespace std;

CentralWidget::CentralWidget(QWidget *parent) : QWidget(parent)
{
    parameter_ = shared_ptr<Parameter>(new Parameter());
    simulator_ = new Simulator(128, 128, parameter_);
    drawer_ = new Drawer(simulator_->get_density_field(), this);

    start_ = new QPushButton("start");
    QGroupBox* control_box = new QGroupBox("control", this);
    QHBoxLayout* control_box_layout = new QHBoxLayout();
    control_box_layout->addWidget(start_);
    control_box->setLayout(control_box_layout);

    use_upwind_ = new QCheckBox("use upwind");
    use_runge_kutta_ = new QCheckBox("use runge kutta");

    QGroupBox* integrate_box = new QGroupBox(this);
    QVBoxLayout* integrate_box_layout = new QVBoxLayout();
    integrate_box_layout->addWidget(use_upwind_);
    integrate_box_layout->addWidget(use_runge_kutta_);
    integrate_box->setLayout(integrate_box_layout);

    color_picker_ = new QPushButton();

    QGroupBox* color_box = new QGroupBox(this);
    QVBoxLayout* color_layout = new QVBoxLayout();
    color_layout->addWidget(color_picker_);
    color_box->setLayout(color_layout);

    time_step_slider_ = new QSlider(Qt::Horizontal);
    visc_slider_ = new QSlider(Qt::Horizontal);
    diffuse_slider_ = new QSlider(Qt::Horizontal);
    dissipation_slider_ = new QSlider(Qt::Horizontal);
    vorticity_slider_ = new QSlider(Qt::Horizontal);

    QGroupBox* parameter_box = new QGroupBox(this);
    QVBoxLayout* parameter_layout = new QVBoxLayout();
    parameter_layout->addWidget(time_step_slider_);
    parameter_layout->addWidget(visc_slider_);
    parameter_layout->addWidget(diffuse_slider_);
    parameter_layout->addWidget(dissipation_slider_);
    parameter_layout->addWidget(vorticity_slider_);

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
}

void CentralWidget::start(){
    simulator_->run();
}

void CentralWidget::next(){
    parameter_->draw_finish = true;
}
