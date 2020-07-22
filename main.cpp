#include "Window.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName("Freud");
    a.setOrganizationName("zyclincoln");
    a.setApplicationVersion("v0.1");
    Window w;

    w.show();
    return a.exec();
}
