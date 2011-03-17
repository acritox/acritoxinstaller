#ifndef partitions_H
#define partitions_H

#include "ui_partitions.h"
#include "../backend.h"

class wpPartitions : public QWizardPage, Ui::wpPartitions
{
  Q_OBJECT

  public:
    wpPartitions(QWidget *parent = 0);
    void initializePage();
    void clearPage();
    int nextId() const;
    bool isComplete() const;
    bool validatePage();
    
  private:
    Backend* backend;
    
  private slots:
    void receivedDataLine(QString data, QString line);
    void updateComplete();
};

#endif // partitions_H
