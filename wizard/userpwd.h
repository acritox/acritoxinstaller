#ifndef userpwd_H
#define userpwd_H

#include "ui_userpwd.h"
#include "../backend.h"

class wpUserPwd : public QWizardPage, Ui::wpUserPwd
{
  Q_OBJECT

  public:
    wpUserPwd(QWidget *parent = 0);
    void initializePage();
    bool isComplete() const;
    bool validatePage();
    
  private:
    Backend* backend;
    bool complete;
    
  private slots:
    void updateStatus();
    
};

#endif // userpwd_H
