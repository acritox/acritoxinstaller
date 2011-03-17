#ifndef mainwizard_H
#define mainwizard_H

#include <QWizard>
#include "backend.h"
#include "busyappfilter.h"

class MainWizard : public QWizard
{
  Q_OBJECT

  public:
    enum {  Page_Welcome,
	    Page_Partitions,
	    Page_PartManSel,
	    Page_PartMan,
	    Page_RootPartition,
	    Page_Bootloader,
	    Page_RootPwd,
	    Page_UserCfg,
	    Page_UserPwd,
	    Page_Network,
	    Page_Summary,
	    Page_Installation };
    MainWizard();
    void reject();
    Backend* backend;
    QSize sizeHint() const;
    
  private:
    BusyAppFilter *filter;
    
  private slots:
    void processCommand(QString command, QString args);
    void backendBusy(bool busy);
    
};

#endif // mainwizard_H
