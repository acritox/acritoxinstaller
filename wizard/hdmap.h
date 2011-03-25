#ifndef hdmap_H
#define hdmap_H

#include "ui_hdmap.h"
#include "../backend.h"

class wpHdMap : public QWizardPage, Ui::wpHdMap
{
  Q_OBJECT

  public:
    wpHdMap(QWidget *parent = 0);
    void initializePage();
    void clearPage();
    bool isComplete() const;
    bool validatePage();
    
  private:
    Backend* backend;
    QStringList filesystems;
    
  private slots:
    void receivedDataLine(QString data, QString line);
    void updateComplete();
    void backendFinishedCommand(QString command);
    
};

#endif // hdmap_H
