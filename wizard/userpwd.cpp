#include <QtGui>
#include "userpwd.h"
#include "../listdelegate.h"
#include "../listitem.h"

wpUserPwd::wpUserPwd(QWidget *parent) : QWizardPage(parent)
{
  setupUi(this);
  connect(password, SIGNAL(textChanged(QString)), this, SLOT(updateStatus()));
  connect(retypePassword, SIGNAL(textChanged(QString)), this, SLOT(updateStatus()));
  backend = Backend::instance();
  updateStatus();
}

void wpUserPwd::initializePage()
{
}

void wpUserPwd::updateStatus()
{
  QString pw1 = password->text();
  QString pw2 = retypePassword->text();
  
  passwordStatus->setPixmap(QPixmap());
  retypePasswordStatus->setPixmap(QPixmap());
  
  complete = true;
  
  if(!pw1.length())
  {
    passwordStatus->setToolTip(tr("Please enter a password!"));
    passwordStatus->setPixmap(QIcon::fromTheme("dialog-warning").pixmap(24,24));
    complete = false;
  }
  else if(pw1.length() < 6)
  {
    passwordStatus->setToolTip(tr("Password must have at least 6 characters!"));
    passwordStatus->setPixmap(QIcon::fromTheme("dialog-warning").pixmap(24,24));
    complete = false;
  }
  else
  {
    passwordStatus->setToolTip(tr("Password OK"));
    passwordStatus->setPixmap(QIcon::fromTheme("dialog-ok").pixmap(24,24));
    
    if(pw1 != pw2)
    {
      retypePasswordStatus->setToolTip(tr("Passwords don't match!"));
      retypePasswordStatus->setPixmap(QIcon::fromTheme("dialog-warning").pixmap(24,24));
      complete = false;
    }
    else
    {
      retypePasswordStatus->setToolTip(tr("Password OK"));
      retypePasswordStatus->setPixmap(QIcon::fromTheme("dialog-ok").pixmap(24,24));
    }
  }
  emit completeChanged();
}

bool wpUserPwd::isComplete() const
{
  return complete;
}

bool wpUserPwd::validatePage()
{
  if(!complete) return false;
  backend->cfg("userpwd", backend->encryptPassword(password->text()));
  backend->cfg("autologin", (autologin->isChecked() ? "on" : "off"));
  return true;
}

