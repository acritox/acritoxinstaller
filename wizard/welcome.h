#ifndef welcome_H
#define welcome_H

#include "ui_welcome.h"
#include "../backend.h"

class wpWelcome : public QWizardPage, Ui::wpWelcome
{
  Q_OBJECT

  public:
    wpWelcome(QWidget *parent = 0);
    bool isComplete() const;
    bool validatePage();
    
  private:
    Backend* backend;
    bool complete;
    void setComplete(bool c);
  
  private slots:
    void backendFinishedCommand(QString command);
    
};

#endif // welcome_H
