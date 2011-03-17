#include <QtGui>
#include "usercfg.h"
#include "../listdelegate.h"
#include "../listitem.h"

wpUserCfg::wpUserCfg(QWidget *parent) : QWizardPage(parent)
{
  setupUi(this);
  connect(realname, SIGNAL(editingFinished()), this, SLOT(realnameChanged()));
  connect(realname, SIGNAL(textChanged(QString)), this, SLOT(updateStatus()));
  connect(username, SIGNAL(textChanged(QString)), this, SLOT(updateStatus()));
  
  QValidator* usernameValidator = new QRegExpValidator(QRegExp("[a-zA-Z0-9-_.]*"), this);
  username->setValidator( usernameValidator );
  
  backend = Backend::instance();
  complete = false;
  updateStatus();
}

void wpUserCfg::initializePage()
{
}

void wpUserCfg::realnameChanged()
{
  if(username->text().length()) return;
  if(realname->text().contains(" "))
    username->setText(backend->cleanUsername(realname->text().section(" ",0,0).toLower()));
  updateStatus();
}

void wpUserCfg::updateStatus()
{
  complete = false;
  if(realname->text().length() && username->text().length()) complete = true;
  emit completeChanged();
}

bool wpUserCfg::isComplete() const
{
  return complete;
}

bool wpUserCfg::validatePage()
{
  if(!complete) return false;
  backend->cfg("username", username->text());
  backend->cfg("realname", realname->text());
  return true;
}

