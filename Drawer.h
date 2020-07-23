#ifndef DRAWER_H
#define DRAWER_H

#include <QWidget>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QPoint>

#include "Field.h"

class Drawer : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit Drawer(const std::vector<Field<double, 1>>& d_field, QWidget *parent = nullptr);
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
    void add_boundary(int x, int y, double dx, double dy);
protected:
    QSize minimumSizeHint() const override;
    QSize sizeHint() const override;
    void initializeGL() override;
    void paintGL() override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    const std::vector<Field<double, 1>>& d_field_;
    std::vector<Field<double, 1>> d_field_buffer_;
    QPoint last_pos_;
    bool record_ = false;
};

#endif // DRAWER_H
