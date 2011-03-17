#ifndef partman_H
#define partman_H

#include "ui_partman.h"
#include "../backend.h"

class wpPartMan : public QWizardPage, Ui::wpPartMan
{
  Q_OBJECT

  public:
    wpPartMan(QWidget *parent = 0);
    void initializePage();
    void clearPage();
    bool isComplete() const;
    bool validatePage();
    
  private:
    Backend* backend;
    bool complete;
    void setComplete(bool c);
    
  private slots:
    void terminalFinished();
};

#endif // partman_H
