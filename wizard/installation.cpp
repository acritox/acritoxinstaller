#include <QtGui>
#include "installation.h"
#include "../listdelegate.h"
#include "../listitem.h"

wpInstallation::wpInstallation(QWidget *parent) : QWizardPage(parent)
{
  setComplete(false);
  setupUi(this);
  backend = Backend::instance();
//   connect(backend, SIGNAL(processExited()), this, SLOT(WeAreDone()));
  listWidget->setItemDelegate(new ListDelegate(this));
  setCommitPage(true);
//   QListWidgetItem *item = new ListItem("Start", "Let's go!", "dialog-ok");
//   listWidget->addItem(item);
}

void wpInstallation::initializePage()
{
  listWidget->clear();
  progressCompleted->setRange(0,12);
  connect(backend, SIGNAL(receivedProgress(int)), this, SLOT(setProgress(int)));
  connect(backend, SIGNAL(receivedCommand(QString,QString)), this, SLOT(receivedCommand(QString,QString)));
  connect(backend, SIGNAL(finishedCommand(QString)), this, SLOT(finishedCommand(QString)));
  backend->exec("do_install");
}

void wpInstallation::cleanupPage()
{
  initializePage();
}

void wpInstallation::setProgress(int percent)
{
  progressCurrent->setRange(0, 100);
  progressCurrent->setValue(percent);
}

void wpInstallation::receivedCommand(QString command, QString args)
{
  if(command != "install_step") return;
  QListWidgetItem *item = new ListItem(args, tr("TODO: use descriptive titles, descriptions and icons in this list..."), "acritoxinstaller");
  listWidget->addItem(item);
  listWidget->scrollToItem(item);
  progressCompleted->setValue(progressCompleted->value()+1);
  progressCurrent->reset();
  progressCurrent->setRange(0,0);
}

void wpInstallation::finishedCommand(QString command)
{
  if(command != "do_install") return;
  progressCompleted->setRange(0,100);
  progressCompleted->setValue(100);
  progressCurrent->setRange(0,100);
  progressCurrent->setValue(100);
  setComplete(true);
}

// void wpInstallation::()
// {
//   QListWidgetItem *item = new ListItem("Finished.", "The backend has finished its job.", "dialog-ok");
//   listWidget->addItem(item);
//   setComplete(true);
// }

void wpInstallation::setComplete(bool c)
{
  complete = c;
  emit completeChanged();
}

bool wpInstallation::isComplete() const
{
  return complete;
}

bool wpInstallation::validatePage()
{
  return complete;
}
