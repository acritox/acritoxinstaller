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
  timer.start();
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
  QString title = args, desc, icon = "acritoxinstaller";
  if(args == "check_partitions_for_install")
  {
    title = tr("Checking the installation configuration...");
    icon = "document-edit-verify";
  }
  else if(args == "prepare_target")
  {
    title = tr("Setting up the installation target...");
    icon = "configure";
  }
  else if(args == "prepare_partitions_for_install")
  {
    title = tr("Preparing partitions for install...");
    desc = tr("Formatting, setting up mountpoints and mounting partitions according to your settings.");
    icon = "partitionmanager";
  }
  else if(args == "copy_system_to_target")
  {
    title = tr("Copying system to installation target...");
    icon = "edit-copy";
  }
  else if(args == "update_fstab_on_target")
  {
    title = tr("Updating /etc/fstab on installation target...");
    icon = "story-editor";
  }
  else if(args == "update_passwd_on_target")
  {
    title = tr("Setting up users and passwords...");
    icon = "user-properties";
  }
  else if(args == "copy_home_to_target")
  {
    title = tr("Copying data for the default user...");
    desc = tr("The home-directory of the live user is copied to the home-directory of the user on the installation target.");
    icon = "user-home";
  }
  else if(args == "copy_etc_to_target")
  {
    title = tr("Copying system settings...");
    desc = tr("Selected files from /etc are copied to the installation target.");
    icon = "folder-development";
  }
  else if(args == "configure_target_update_files")
  {
    title = tr("Updating configuration files...");
    icon = "run-build-configure";
  }
  else if(args == "configure_target_purge_live_only_stuff")
  {
    title = tr("Removing live-only stuff...");
    desc = tr("Removing packages that are only needed on a live system from the installation (e.g. this installer)");
    icon = "user-trash-full";
  }
  else if(args == "configure_target_services")
  {
    title = tr("Configuring services...");
    desc = tr("Setting up the services which get started automatically when booting the installation.");
    icon = "services";
  }
  else if(args == "install_bootmanager_to_target")
  {
    title = tr("Installing bootmanager to the selected target...");
    desc = tr("Generating an initial ramdisk and installing %1 to %2")
      .arg(backend->cfg("bootloader")).arg(backend->cfg("bootloader_target"));
    icon = "system-run";
  }
  else if(args == "cleanup")
  {
    title = tr("Cleaning up...");
    icon = "run-build-clean";
  }
  QListWidgetItem *item = new ListItem(title, desc, icon);
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
  int elapsed = timer.elapsed()/1000;
  QString desc = tr("The installation took %1m%2s.").arg(elapsed/60).arg(elapsed % 60, 2, 10, QLatin1Char('0'));
  QListWidgetItem *item = new ListItem(tr("Finished."), desc, "dialog-ok-apply");
  listWidget->addItem(item);
  listWidget->scrollToItem(item);
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
