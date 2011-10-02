#include <QtGui>
#include "rootpartition.h"
#include "../listdelegate.h"
#include "../listitem.h"
#include "../mainwizard.h"

wpRootPartition::wpRootPartition(QWidget *parent) : QWizardPage(parent)
{
  setupUi(this);
  backend = Backend::instance();
  connect(backend, SIGNAL(receivedDataLine(QString,QString)), this, SLOT(receivedDataLine(QString,QString)));
  connect(backend, SIGNAL(receivedCommand(QString,QString)), this, SLOT(receivedCommand(QString,QString)));
  connect(backend, SIGNAL(finishedCommand(QString)), this, SLOT(backendFinishedCommand(QString)));
  connect(rootPartitionDev, SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)), this, SLOT(updateComplete()));
  connect(chkAdvanced, SIGNAL(stateChanged(int)), this, SLOT(updateComplete()));
  rootPartitionDev->setItemDelegate(new ListDelegate(this));
  
  checkPassed = false;
}

void wpRootPartition::initializePage()
{
  clearPage();
}

void wpRootPartition::clearPage()
{
  backend->exec("send_possible_root_partitions");
  rootPartitionDev->clear();
  backend->exec("send_possible_root_filesystems");
  rootPartitionFs->clear();
  
  checkPassed = false;
}

void wpRootPartition::receivedDataLine(QString data, QString line)
{
  if(data == "possible_root_partitions")
  {
    QString dev = line.section(" ",0,0);
    qlonglong size = line.section(" ",1,1).toLongLong();
    QString desc = QString("%1 (%L2)").arg(backend->sizeToString(size)).arg(size);
    desc += "<br />" + line.section(" ",2);
    QListWidgetItem *item = new ListItem(dev, desc, "drive-harddisk", dev);
    rootPartitionDev->addItem(item);
  }
  if(data == "possible_root_filesystems")
  {
    rootPartitionFs->addItem(line);
  }
}

void wpRootPartition::updateComplete()
{
  emit completeChanged();
}

bool wpRootPartition::isComplete() const
{
  if(!chkAdvanced->isChecked() && !rootPartitionDev->currentItem()) return false;
  return true;
}

void wpRootPartition::backendFinishedCommand(QString command)
{
  if(this->wizard()->currentId() != MainWizard::Page_RootPartition) return;
  
  if(command == "check_partitions_for_install" && checkPassed)
  {
    this->wizard()->next();
  }
}

void wpRootPartition::receivedCommand(QString command, QString args)
{
  if(this->wizard()->currentId() != MainWizard::Page_RootPartition) return;
  
  if(command == "error") checkPassed = false;
}

bool wpRootPartition::validatePage()
{
  if(!isComplete()) return false;
  if(checkPassed)
  {
    checkPassed = false;
    return true;
  }
  if(rootPartitionDev->currentItem())
    backend->exec(QString("hdmap_set %1:/:%2:auto")
	  .arg(rootPartitionDev->currentItem()->data(ListItem::ItemData).toString())
	  .arg(chkFormat->isChecked() ? rootPartitionFs->currentText() : ""));
    backend->exec("fill_hdmap");
  
  if(chkAdvanced->isChecked()) return true;
  
  backend->exec("check_partitions_for_install");
  checkPassed = true; // if an error occurrs receivedCommand will set it to false.
  return false; 
}

int wpRootPartition::nextId() const
{
    if(chkAdvanced->isChecked())
      return MainWizard::Page_HdMap;
    else
      return MainWizard::Page_Bootloader;
}
