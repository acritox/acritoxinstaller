#include <QtGui>
#include "partman.h"
#include "config.h"
#include "../listdelegate.h"
#include "../listitem.h"

wpPartMan::wpPartMan(QWidget *parent) : QWizardPage(parent)
{
  setupUi(this);
  backend = Backend::instance();
}

void wpPartMan::initializePage()
{
  clearPage();
}

void wpPartMan::clearPage()
{
  setComplete(false);
  delete terminal;
  delete gridLayout;
  delete label;
  setupUi(this);
  connect(terminal, SIGNAL(finished()), this, SLOT(terminalFinished()));
  QStringList args;
  args << "/bin/bash" << BACKEND_PATH << "-e" << "run_partmgr" << backend->cfg("partman_program") << backend->cfg("partman_disk");
  terminal->setArgs(args);
  terminal->startShellProgram();
  terminal->setFocus(Qt::OtherFocusReason);
}

void wpPartMan::terminalFinished()
{
  terminal->releaseKeyboard();
  terminal->clearFocus();
  setComplete(true);
}

void wpPartMan::setComplete(bool c)
{
  complete = c;
  emit completeChanged();
}

bool wpPartMan::isComplete() const
{
  return complete;
}

bool wpPartMan::validatePage()
{
  return complete;
}

