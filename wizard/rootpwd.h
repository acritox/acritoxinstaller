#ifndef rootpwd_H
#define rootpwd_H

#include "ui_rootpwd.h"
#include "../backend.h"

class wpRootPwd : public QWizardPage, Ui::wpRootPwd
{
  Q_OBJECT

  public:
    wpRootPwd(QWidget *parent = 0);
    void initializePage();
    bool isComplete() const;
    bool validatePage();
    
  private:
    Backend* backend;
    bool complete;
    
  private slots:
    void updateStatus();
    
};

#endif // rootpwd_H
