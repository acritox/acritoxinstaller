#include <QtGui/QApplication>
#include "mainwizard.h"

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    MainWizard *l = new MainWizard;
    l->show();
    return app.exec();
}
