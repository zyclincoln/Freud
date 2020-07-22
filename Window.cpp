#include "Window.h"

Window::Window(QWidget *parent)
    : QMainWindow(parent)
{
    setCentralWidget(&this->centralWidget_);
}

Window::~Window()
{
}

