#include <QtGui>
#include "welcome.h"
#include "config.h"
#include "../listdelegate.h"
#include "../listitem.h"

wpWelcome::wpWelcome(QWidget *parent) : QWizardPage(parent)
{
  setupUi(this);
  backend = Backend::instance();
  connect(backend, SIGNAL(finishedCommand(QString)), this, SLOT(backendFinishedCommand(QString)));
  setComplete(false);
  textBrowser->setSource(QString("%1/html/%2.html").arg(BACKEND_DIR).arg(tr("welcome")));
  backend->exec("init_installer");
}

void wpWelcome::backendFinishedCommand(QString command)
{
  if(command == "init_installer") setComplete(true);
}

void wpWelcome::setComplete(bool c)
{
  complete = c;
  emit completeChanged();
}

bool wpWelcome::isComplete() const
{
  return complete;
}

bool wpWelcome::validatePage()
{
  return complete;
}

