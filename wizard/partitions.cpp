#include <QtGui>
#include "partitions.h"
#include "../listdelegate.h"
#include "../listitem.h"
#include "../mainwizard.h"

wpPartitions::wpPartitions(QWidget *parent) : QWizardPage(parent)
{
  setupUi(this);
  backend = Backend::instance();
  connect(listWidget, SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)), this, SLOT(updateComplete()));
  listWidget->setItemDelegate(new ListDelegate(this));
}

void wpPartitions::initializePage()
{
  listWidget->clear();
  listWidget->addItem(new ListItem(tr("Manual partitioning"), tr("You will have to do the partitioning with a partition-manager manually. This way you have more possibilities to adjust the installation to your needs. The automatic partitioning lacks several features and is only a (computer generated) list of some possible options."), "partitionmanager", "manual"));
  listWidget->addItem(new ListItem(tr("Already partitioned"), tr("If you have already partitioned for the installation and don't want to start a partition manager then select this."), "drive-harddisk", "already"));
  listWidget->setCurrentRow(0);
  clearPage();
}

void wpPartitions::clearPage()
{
}

void wpPartitions::receivedDataLine(QString data, QString line)
{
  if(data == "install_choice")
  {
    listWidget->addItem(new ListItem(line, line, "tools-wizard", line.section(" ",0,0)));
  }
}

void wpPartitions::updateComplete()
{
  emit completeChanged();
}

bool wpPartitions::isComplete() const
{
  if(!listWidget->currentItem()) return false;
  return true;
}

bool wpPartitions::validatePage()
{
  if(!isComplete()) return false;
//   backend->cfg("install_choice", listWidget->currentItem()->data(ListItem::ItemData).toString());
  return true;
}

int wpPartitions::nextId() const
{
  if(listWidget->currentItem())
  {
    QString choice = listWidget->currentItem()->data(ListItem::ItemData).toString();
    if(choice == "manual")
      return MainWizard::Page_PartManSel;
    else if(choice == "already")
      return MainWizard::Page_RootPartition;
  }
}
