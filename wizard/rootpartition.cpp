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
  connect(rootPartitionDev, SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)), this, SLOT(updateComplete()));
  connect(chkAdvanced, SIGNAL(stateChanged(int)), this, SLOT(updateComplete()));
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
}

void wpRootPartition::receivedDataLine(QString data, QString line)
{
  if(data == "possible_root_partitions")
  {
    QListWidgetItem *item = new QListWidgetItem(QIcon::fromTheme("drive-harddisk"), line);
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

bool wpRootPartition::validatePage()
{
  if(!isComplete()) return false;
  if(rootPartitionDev->currentItem())
    backend->exec(QString("hdmap_set %1:/:%2:auto")
	  .arg(rootPartitionDev->currentItem()->text().section(" ",0,0))
	  .arg(chkFormat->isChecked() ? rootPartitionFs->currentText() : ""));
    backend->exec("fill_hdmap");
  return true;
}

int wpRootPartition::nextId() const
{
    if(chkAdvanced->isChecked())
      return MainWizard::Page_HdMap;
    else
      return MainWizard::Page_Bootloader;
}
