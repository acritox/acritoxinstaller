#ifndef installation_H
#define installation_H

#include "ui_installation.h"
#include "../backend.h"

class wpInstallation : public QWizardPage, Ui::wpInstallation
{
  Q_OBJECT

  public:
    wpInstallation(QWidget *parent = 0);
    void initializePage();
    void cleanupPage();
    bool isComplete() const;
    bool validatePage();
    
  private:
    Backend* backend;
    bool complete;
    void setComplete(bool c);
    
  private slots:
    void setProgress(int percent);
    void receivedCommand(QString command, QString args);
    void finishedCommand(QString command);
};

#endif // installation_H
