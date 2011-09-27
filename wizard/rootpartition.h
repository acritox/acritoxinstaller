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
    int nextId() const;
    bool isComplete() const;
    bool validatePage();
    
  private:
    Backend* backend;
    bool checkPassed;
    
  private slots:
    void receivedDataLine(QString data, QString line);
    void updateComplete();
    void backendFinishedCommand(QString command);
    void receivedCommand(QString command, QString args);
    
};

#endif // rootpartition_H
