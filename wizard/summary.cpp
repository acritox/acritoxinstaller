#include <QtGui>
#include "summary.h"
#include "../listdelegate.h"
#include "../listitem.h"

wpSummary::wpSummary(QWidget *parent) : QWizardPage(parent)
{
  setComplete(true);
  setupUi(this);
  backend = Backend::instance();
//   connect(backend, SIGNAL(processExited()), this, SLOT(WeAreDone()));
  listWidget->setItemDelegate(new ListDelegate(this));
  setCommitPage(true);
  setButtonText(QWizard::CommitButton, tr("Start Installation"));
//   QListWidgetItem *item = new ListItem("Start", "Let's go!", "dialog-ok");
//   listWidget->addItem(item);
}

void wpSummary::initializePage()
{
  listWidget->clear();
  listWidget->addItem(new ListItem(tr("Installation type"), tr("HD install"), "acritoxinstaller"));
  QStringList hdMapList = backend->cfg("hdmap").split("\n");
  for (QStringList::Iterator it = hdMapList.begin(); it != hdMapList.end(); ++it)
  {
    QString line = *it;
    QString device = line.section(":",0,0);
    QString mountpoint = line.section(":",1,1);
    QString filesystem = line.section(":",2,2);
    QString automount = line.section(":",3,3);
    QString title = tr("%1 will be used as %2").arg(device).arg(mountpoint);
    QString description;
    if(!mountpoint.length()) continue;
    if(filesystem.length()) description = QString("<li>") + tr("It will be formatted with %1").arg(filesystem) + QString("</li>");
    if(automount == "auto") description += QString("<li>") + tr("It will be mounted automatically on boot") + QString("</li>");
    listWidget->addItem(new ListItem(title, "<ul>" + description + "</ul>", "drive-harddisk"));
  }
  listWidget->addItem(new ListItem(tr("Bootloader"), tr("%1 will be installed to %2").arg(backend->cfg("bootloader")).arg(backend->cfg("bootloader_target")), "system-run"));
}

void wpSummary::cleanupPage()
{
  initializePage();
}

// void wpSummary::()
// {
//   QListWidgetItem *item = new ListItem("Finished.", "The backend has finished its job.", "dialog-ok");
//   listWidget->addItem(item);
//   setComplete(true);
// }

void wpSummary::setComplete(bool c)
{
  complete = c;
  emit completeChanged();
}

bool wpSummary::isComplete() const
{
  return complete;
}

bool wpSummary::validatePage()
{
  return complete;
}
