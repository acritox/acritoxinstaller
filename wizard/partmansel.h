#ifndef partmansel_H
#define partmansel_H

#include "ui_partmansel.h"
#include "../backend.h"

class wpPartManSel : public QWizardPage, Ui::wpPartManSel
{
  Q_OBJECT

  public:
    wpPartManSel(QWidget *parent = 0);
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

#endif // partmansel_H
