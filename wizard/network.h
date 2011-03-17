#ifndef network_H
#define network_H

#include "ui_network.h"
#include "../backend.h"

class wpNetwork : public QWizardPage, Ui::wpNetwork
{
  Q_OBJECT

  public:
    wpNetwork(QWidget *parent = 0);
    void initializePage();
    bool isComplete() const;
    bool validatePage();
    
  private:
    Backend* backend;
    
  private slots:
    void updateStatus();
    
};

#endif // network_H
