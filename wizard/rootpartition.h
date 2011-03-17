#ifndef rootpartition_H
#define rootpartition_H

#include "ui_rootpartition.h"
#include "../backend.h"

class wpRootPartition : public QWizardPage, Ui::wpRootPartition
{
  Q_OBJECT

  public:
    wpRootPartition(QWidget *parent = 0);
    void initializePage();
    void clearPage();
    bool isComplete() const;
    bool validatePage();
    
  private:
    Backend* backend;
    
  private slots:
    void receivedDataLine(QString data, QString line);
    void updateComplete();
    
};

#endif // rootpartition_H
