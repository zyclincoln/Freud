#include <iostream>
#include "Window.h"

using namespace std;

Window::Window(QWidget *parent)
    : QMainWindow(parent)
{
    setCentralWidget(&this->centralWidget_);
}

Window::~Window()
{
}

