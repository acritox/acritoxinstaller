#ifndef usercfg_H
#define usercfg_H

#include "ui_usercfg.h"
#include "../backend.h"

class wpUserCfg : public QWizardPage, Ui::wpUserCfg
{
  Q_OBJECT

  public:
    wpUserCfg(QWidget *parent = 0);
    void initializePage();
    bool isComplete() const;
    bool validatePage();
    
  private:
    Backend* backend;
    bool complete;
    
  private slots:
    void realnameChanged();
    void updateStatus();
    
};

#endif // usercfg_H
