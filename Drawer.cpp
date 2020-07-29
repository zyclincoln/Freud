#include <QDebug>
#include <QMouseEvent>
#include <iostream>

#include "Drawer.h"

using namespace std;

Drawer::Drawer(const vector<Field<double, 1>>& d_field, QWidget *parent) :
    QOpenGLWidget(parent), d_field_(d_field)
{
    d_field_buffer_ = d_field_;
}

Drawer::~Drawer()
{

}

void Drawer::redraw() {
    d_field_buffer_ = d_field_;
    emit draw_finish();
    paintGL();
    update();
}

void Drawer::mousePressEvent(QMouseEvent *event){
    last_pos_ = event->pos();
    record_ = true;
}

void Drawer::mouseReleaseEvent(QMouseEvent *event){
    record_ = false;
    //QPoint cur_pos = event->pos();

    //// calculate grid
    //int x = cur_pos.x()*1.0/this->width() * d_field_[0].get_width();
    //int y = cur_pos.y()*1.0/this->height() * d_field_[0].get_height();

    ////double dx = (cur_pos.x() - last_pos_.x())*1.0/this->width();
    ////double dy = (cur_pos.y() - last_pos_.y())*1.0/this->height();
    //double dx = (last_pos_.x() - cur_pos.x()) * 1.0 / this->width();
    //double dy = (last_pos_.y() - cur_pos.y()) * 1.0 / this->height();
    //double r = sqrt(dx * dx + dy * dy);
    //dx /= r; dy /= r;
    //emit add_boundary(x, y, dx, dy);
}

void Drawer::mouseMoveEvent(QMouseEvent* event)
{
    if (record_) {
        QPoint cur_pos = event->pos();
        int x = cur_pos.x() * 1.0 / this->width() * d_field_[0].get_width();
        int y = cur_pos.y() * 1.0 / this->height() * d_field_[0].get_height();
        double dx = (last_pos_.x() - cur_pos.x()) * 1.0 / this->width();
        double dy = (last_pos_.y() - cur_pos.y()) * 1.0 / this->height();
        double r = sqrt(dx * dx + dy * dy);
        dx /= r; dy /= r;

        last_pos_ = cur_pos;
        emit add_boundary(x, y, dx, dy);
    }
}

QSize Drawer::minimumSizeHint() const
{
    return QSize(50, 50);
}

QSize Drawer::sizeHint() const
{
    return QSize(800, 800);
}

void Drawer::initializeGL(){
    initializeOpenGLFunctions();
    glClearColor(0, 0, 0, 1);
}

void Drawer::paintGL(){
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);

    glViewport(0, 0, this->width(), this->height());
    glLoadIdentity();
    glOrtho(0, this->width(), this->height(), 0, -1.0, 1.0);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    int yN = d_field_buffer_[0].get_height(), xN = d_field_buffer_[0].get_width();
    double grid_width = 1.0/(xN-2)*this->width(), grid_height = 1.0/(yN-2)*this->height();
    glBegin(GL_TRIANGLES);
    for(int y = 1; y < yN-1; ++y){
        for(int x = 1; x < xN-1; ++x){
            glColor3f(d_field_buffer_[0](x, y, 0),
                      d_field_buffer_[1](x, y, 0),
                      d_field_buffer_[2](x, y, 0));
            glVertex2f((x - 1) * grid_width, (y - 1) * grid_height);
            glVertex2f((x - 1) * grid_width, y * grid_height);
            glVertex2f(x * grid_width, (y - 1) * grid_height);
            glVertex2f((x - 1) * grid_width, y * grid_height);
            glVertex2f(x * grid_width, (y - 1) * grid_height);
            glVertex2f(x * grid_width, y * grid_height);
        }
    }
    glEnd();
}
