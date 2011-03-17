#include <QtGui>
#include "bootloader.h"
#include "../listdelegate.h"
#include "../listitem.h"

wpBootloader::wpBootloader(QWidget *parent) : QWizardPage(parent)
{
  setupUi(this);
  backend = Backend::instance();
  connect(backend, SIGNAL(receivedDataLine(QString,QString)), this, SLOT(receivedDataLine(QString,QString)));
  connect(backend, SIGNAL(finishedCommand(QString)), this, SLOT(backendFinishedCommand(QString)));
  connect(bootloader, SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)), this, SLOT(updateComplete()));
  connect(bootloaderTarget, SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)), this, SLOT(updateComplete()));
  //bootloader->setItemDelegate(new ListDelegate(this));
}

void wpBootloader::initializePage()
{
  clearPage();
}

void wpBootloader::clearPage()
{
  backend->exec("send_bootloaders");
  bootloader->clear();
  backend->exec("send_bootloader_targets");
  bootloaderTarget->clear();
}

void wpBootloader::receivedDataLine(QString data, QString line)
{
  if(data == "bootloaders")
  {
    QListWidgetItem *item = new QListWidgetItem(QIcon::fromTheme("system-run"), line);
    bootloader->addItem(item);
  }
  if(data == "bootloader_targets")
  {
    QListWidgetItem *item = new QListWidgetItem(QIcon::fromTheme("drive-harddisk"), line);
    bootloaderTarget->addItem(item);
  }
}

void wpBootloader::backendFinishedCommand(QString command)
{
  if(command == "send_bootloaders")
  {
    bootloader->setCurrentRow(0);
  }
  if(command == "send_bootloader_targets")
  {
    bootloaderTarget->setCurrentRow(0);
  }
}

void wpBootloader::updateComplete()
{
  emit completeChanged();
}

bool wpBootloader::isComplete() const
{
  if(!bootloader->currentItem()) return false;
  if(!bootloaderTarget->currentItem()) return false;
  return true;
}

bool wpBootloader::validatePage()
{
  if(!isComplete()) return false;
  backend->cfg("bootloader", bootloader->currentItem()->text().section(" ",0,0).toLower());
  backend->cfg("bootloader_target", bootloaderTarget->currentItem()->text().section(" ",0,0).toLower());
  return true;
}
