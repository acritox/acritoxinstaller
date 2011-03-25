#include <QtGui>
#include "hdmap.h"
#include "../listdelegate.h"
#include "../listitem.h"

wpHdMap::wpHdMap(QWidget *parent) : QWizardPage(parent)
{
  setupUi(this);
  backend = Backend::instance();
  connect(backend, SIGNAL(receivedDataLine(QString,QString)), this, SLOT(receivedDataLine(QString,QString)));
  connect(backend, SIGNAL(finishedCommand(QString)), this, SLOT(backendFinishedCommand(QString)));
  tableWidget->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
}

void wpHdMap::initializePage()
{
  clearPage();
}

void wpHdMap::clearPage()
{
  backend->exec("send_possible_root_filesystems");
  filesystems.clear();
  backend->exec("fill_hdmap");
  
  tableWidget->setColumnCount(4);
  tableWidget->setHorizontalHeaderLabels(QStringList() << tr("Partition") << tr("Mountpoint") << tr("Format with") << tr("Automount"));
  
//   tableWidget->horizontalHeader()->setResizeMode(1,QHeaderView::Stretch);
//   tableWidget->horizontalHeader()->setResizeMode(2,QHeaderView::Stretch);
//   tableWidget->horizontalHeader()->setResizeMode(3,QHeaderView::Stretch);
}

void wpHdMap::receivedDataLine(QString data, QString line)
{
  if(data == "possible_root_filesystems")
  {
    filesystems << line.trimmed();
  }
}

void wpHdMap::backendFinishedCommand(QString command)
{
  if(command == "fill_hdmap")
  {
  
    QStringList hdmap = backend->cfg("hdmap").split("\n");
  
    tableWidget->setRowCount(hdmap.count());  
    for(int row = 0; row < hdmap.count(); row++)
    {
      QTableWidgetItem *partition = new QTableWidgetItem(hdmap.at(row).section(":",0,0));
      partition->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
      tableWidget->setItem(row, 0, partition);
      QLineEdit *mountpoint = new QLineEdit(this);
      mountpoint->setText(hdmap.at(row).section(":",1,1));
      tableWidget->setCellWidget(row, 1, mountpoint);
      QComboBox *filesystem = new QComboBox(this);
      filesystem->addItem("");
      filesystem->addItems(filesystems);
      filesystem->setCurrentIndex(filesystems.indexOf(hdmap.at(row).section(":",2,2))+1);
      tableWidget->setCellWidget(row, 2, filesystem);
      QCheckBox *automount = new QCheckBox(this);
      automount->setText(tr("mount on boot"));
      if(hdmap.at(row).section(":",3,3) == "auto") automount->setChecked(true); else automount->setChecked(false);
      tableWidget->setCellWidget(row, 3, automount);
    }
  }
}

void wpHdMap::updateComplete()
{
  emit completeChanged();
}

bool wpHdMap::isComplete() const
{
//  if(!rootPartitionDev->currentItem()) return false;
  return true;
}

bool wpHdMap::validatePage()
{
  if(!isComplete()) return false;
  QStringList* hdmap = new QStringList();
  for(int row = 0; row < tableWidget->rowCount(); row++)
  {
    hdmap->append(QString("%1:%2:%3:%4")
      .arg(tableWidget->item(row,0)->text())
      .arg(qobject_cast<QLineEdit*>(tableWidget->cellWidget(row,1))->text())
      .arg(qobject_cast<QComboBox*>(tableWidget->cellWidget(row,2))->currentText())
      .arg(qobject_cast<QCheckBox*>(tableWidget->cellWidget(row,3))->isChecked() ? "auto" : ""));
  }
  backend->cfg("hdmap", hdmap->join("\n"));
//   backend->exec(QString("hdmap_set %1").arg(hdmap->join("\n")));
  return true;
}
