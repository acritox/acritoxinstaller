#include <QtGui/QApplication>
#include <QLocale>
#include <QLibraryInfo>
#include <QTranslator>
#include "mainwizard.h"

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    QString locale = QLocale::system().name();
    QTranslator *translator;
    
    translator = new QTranslator(&app);
    translator->load(QString("qt_")+locale, QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    app.installTranslator(translator);
    
    translator = new QTranslator(&app);
    translator->load(QString("acritoxinstaller_")+locale, QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    app.installTranslator(translator);
    
    MainWizard *l = new MainWizard;
    l->show();
    return app.exec();
}
