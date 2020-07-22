#ifndef WINDOW_H
#define WINDOW_H

#include <QMainWindow>
#include "CentralWidget.h"

class Window : public QMainWindow
{
    Q_OBJECT
public:
    Window(QWidget *parent = nullptr);
    ~Window();
private:
    CentralWidget centralWidget_;
};
#endif // WINDOW_H
