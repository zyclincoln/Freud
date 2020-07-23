#include <QColorDialog>

#include "ColorPicker.h"

ColorPicker::ColorPicker(QWidget *parent): QPushButton(parent)
{
    connect(this, SIGNAL(clicked()), this, SLOT(changeColor()));
}

void ColorPicker::updateColor(){
    setStyleSheet("background-color: " + color_.name());
}

void ColorPicker::changeColor(){
    QColor new_color = QColorDialog::getColor(color_, parentWidget());
    if(new_color != color_){
        setColor(new_color);
    }
}

void ColorPicker::setColor(const QColor &color){
    this->color_ = color;
    updateColor();
}

const QColor& ColorPicker::getColor(){
    return color_;
}
