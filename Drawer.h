#ifndef DRAWER_H
#define DRAWER_H

#include <QWidget>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>

#include "Field.h"

class Drawer : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit Drawer(const Field<double, 1>& d_field, QWidget *parent = nullptr);
    ~Drawer();

public slots:
    void redraw(){
        d_field_buffer_ = d_field_;
        emit draw_finish();
        paintGL();
        update();
    }

signals:
    void draw_finish();

protected:
    QSize minimumSizeHint() const override;
    QSize sizeHint() const override;
    void initializeGL() override;
    void paintGL() override;
signals:

private:
    const Field<double, 1>& d_field_;
    Field<double, 1> d_field_buffer_;
};

#endif // DRAWER_H
