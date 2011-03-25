#include <QtGui>
#include "mainwizard.h"
#include "listdelegate.h"
#include "listitem.h"
#include "wizard/welcome.h"
#include "wizard/partitions.h"
#include "wizard/partmansel.h"
#include "wizard/partman.h"
#include "wizard/rootpartition.h"
#include "wizard/hdmap.h"
#include "wizard/bootloader.h"
#include "wizard/rootpwd.h"
#include "wizard/usercfg.h"
#include "wizard/userpwd.h"
#include "wizard/network.h"
#include "wizard/summary.h"
#include "wizard/installation.h"

MainWizard::MainWizard()
{
  setMinimumSize(QSize(640,480));
  
  backend = Backend::instance();
  backend->runBackend();
    
  connect(backend, SIGNAL(isBusy(bool)), this, SLOT(backendBusy(bool)));
  connect(backend, SIGNAL(receivedCommand(QString,QString)), this, SLOT(processCommand(QString,QString)));
  
  setPage(Page_Welcome, new wpWelcome(this));
  setPage(Page_Partitions, new wpPartitions(this));
  setPage(Page_PartManSel, new wpPartManSel(this));
  setPage(Page_PartMan, new wpPartMan(this));
  setPage(Page_RootPartition, new wpRootPartition(this));
  setPage(Page_HdMap, new wpHdMap(this));
  setPage(Page_Bootloader, new wpBootloader(this));
  setPage(Page_RootPwd, new wpRootPwd(this));
  setPage(Page_UserCfg, new wpUserCfg(this));
  setPage(Page_UserPwd, new wpUserPwd(this));
  setPage(Page_Network, new wpNetwork(this));
  setPage(Page_Summary, new wpSummary(this));
  setPage(Page_Installation, new wpInstallation(this));
  setStartId(Page_Welcome);
  setWizardStyle(ModernStyle);
  setWindowTitle(tr("Installer"));

  filter = new BusyAppFilter;
  
  setPixmap(QWizard::LogoPixmap, QIcon::fromTheme("acritoxinstaller").pixmap(64,64));
}

void MainWizard::reject()
{
  backend->exitBackend();
  QWizard::reject();
}

void MainWizard::backendBusy(bool busy)
{
  qDebug((QString("MainWizard::backendBusy: ")+(busy ? "yes" : "no")).toUtf8());
  if(!busy)
  {
    QApplication::restoreOverrideCursor();
    QApplication::instance()->removeEventFilter(filter);
  }
  else if(!QApplication::overrideCursor())
  {
    QApplication::setOverrideCursor(Qt::WaitCursor);
    QApplication::instance()->installEventFilter(filter);
  }
}

void MainWizard::processCommand(QString command, QString args)
{
  if(command == "loadpage")
  {
    QList<int> pages = pageIds();
    for(int i = 0; i < pages.size(); ++i)
      if(page(i)->objectName() == args)
      {
	break;
      }
  }
}

QSize MainWizard::sizeHint() const
{
  return QSize(800,600);
}
