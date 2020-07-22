#include <QDebug>

#include "Drawer.h"

Drawer::Drawer(const Field<double, 1>& d_field, QWidget *parent) :
    QOpenGLWidget(parent), d_field_(d_field)
{

}

Drawer::~Drawer()
{

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

    int yN = d_field_.get_height()-2, xN = d_field_.get_width()-2;
    int grid_width = 1.f/(xN-2)*this->width(), grid_height = 1.f/(yN-2)*this->height();
    for(int y = 1; y < yN-1; ++y){
        for(int x = 1; x < xN-1; ++x){
            glColor4d(0.f, d_field_(x, y, 0), d_field_(x, y, 0), 1.f);
            glRectf((x-1)*grid_width, (y-1)*grid_height,
                    x*grid_width, y*grid_height);
        }
    }
}
