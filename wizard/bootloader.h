#ifndef bootloader_H
#define bootloader_H

#include "ui_bootloader.h"
#include "../backend.h"

class wpBootloader : public QWizardPage, Ui::wpBootloader
{
  Q_OBJECT

  public:
    wpBootloader(QWidget *parent = 0);
    void initializePage();
    void clearPage();
    bool isComplete() const;
    bool validatePage();
    
  private:
    Backend* backend;
    
  private slots:
    void receivedDataLine(QString data, QString line);
    void backendFinishedCommand(QString command);
    void updateComplete();
    
};

#endif // bootloader_H
