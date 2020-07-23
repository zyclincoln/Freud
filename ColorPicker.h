#ifndef COLORPICKER_H
#define COLORPICKER_H

#include <QObject>
#include <QPushButton>

class ColorPicker : public QPushButton
{
    Q_OBJECT
public:
    explicit ColorPicker(QWidget *parent = nullptr);
    void setColor(const QColor& color);
    const QColor& getColor();
signals:
public slots:
    void updateColor();
    void changeColor();
private:
    QColor color_;
};

#endif // COLORPICKER_H
