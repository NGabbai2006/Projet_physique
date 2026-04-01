#include "Projet_physique.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Projet_physique window;
    window.show();
    return app.exec();
}
