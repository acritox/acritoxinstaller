#include <QtGui>
#include "partmansel.h"
#include "../listdelegate.h"
#include "../listitem.h"

wpPartManSel::wpPartManSel(QWidget *parent) : QWizardPage(parent)
{
  setupUi(this);
  backend = Backend::instance();
  connect(backend, SIGNAL(receivedDataLine(QString,QString)), this, SLOT(receivedDataLine(QString,QString)));
  connect(partMan, SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)), this, SLOT(updateComplete()));
  connect(partDisk, SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)), this, SLOT(updateComplete()));
  partMan->setItemDelegate(new ListDelegate(this));
  partDisk->setItemDelegate(new ListDelegate(this));
}

void wpPartManSel::initializePage()
{
  clearPage();
}

void wpPartManSel::clearPage()
{
  backend->exec("send_partition_managers");
  partMan->clear();
  backend->exec("send_list_of_disks");
  partDisk->clear();
}

void wpPartManSel::receivedDataLine(QString data, QString line)
{
  if(data == "partition_managers")
  {
    QString app = line.section(" ",0,0);
    QString title, desc, icon;
    if(app == "cfdisk")
    {
      title = tr("cfdisk");
      desc = tr("cfdisk is a curses-based partition editor. It is text-only (curses), but it is easy to use.");
      icon = "terminal";
    }
    else if(app == "fdisk")
    {
      title = tr("fdisk");
      desc = tr("fdisk is a classic partition table manipulator for Linux. It has a Command Line Interface.");
      icon = "terminal";
    }
    else if(app == "gparted")
    {
      title = tr("GParted");
      desc = tr("GParted is the Gnome Partition Editor application. It has a Graphical User Interface. [recommended]");
      icon = "gparted";
    }
    else if(app == "qtparted")
    {
      title = tr("QtParted");
      desc = tr("QTParted is a Partition Magic clone, so it has a GUI. Sometimes it's a bit buggy, so it is not recommended to use for huge changes on the partition table.");
      icon = "qtparted";
    }
    else
    {
      title = app;
      desc = tr("No description available...");
      icon = "partitionmanager";
    }
    QListWidgetItem *item = new ListItem(title, desc, icon, app);
    partMan->addItem(item);
  }
  if(data == "list_of_disks")
  {
    QString dev = line.section(" ",0,0);
    qlonglong size = line.section(" ",1,1).toLongLong();
    QString desc = QString("%1 (%L2)").arg(backend->sizeToString(size)).arg(size);
    QListWidgetItem *item = new ListItem(dev, desc, "drive-harddisk", dev);
    partDisk->addItem(item);
  }
}

void wpPartManSel::updateComplete()
{
  emit completeChanged();
}

bool wpPartManSel::isComplete() const
{
  if(!partMan->currentItem()) return false;
  if(!partDisk->currentItem()) return false;
  return true;
}

bool wpPartManSel::validatePage()
{
  if(!isComplete()) return false;
  backend->cfg("partman_program", partMan->currentItem()->data(ListItem::ItemData).toString());
  backend->cfg("partman_disk", partDisk->currentItem()->data(ListItem::ItemData).toString());
  return true;
}
