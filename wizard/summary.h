#ifndef summary_H
#define summary_H

#include "ui_summary.h"
#include "../backend.h"

class wpSummary : public QWizardPage, Ui::wpSummary
{
  Q_OBJECT

  public:
    wpSummary(QWidget *parent = 0);
    void initializePage();
    void cleanupPage();
    bool isComplete() const;
    bool validatePage();
    
  private:
    Backend* backend;
    bool complete;
    void setComplete(bool c);
    
  private slots:
    
    
};

#endif // summary_H
