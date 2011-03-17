#include <QtGui>
#include "network.h"
#include "../listdelegate.h"
#include "../listitem.h"

wpNetwork::wpNetwork(QWidget *parent) : QWizardPage(parent)
{
  setupUi(this);
  connect(hostname, SIGNAL(textChanged(QString)), this, SLOT(updateStatus()));
  
  QValidator* hostnameValidator = new QRegExpValidator(QRegExp("[a-zA-Z0-9][a-zA-Z0-9.-]*[a-zA-Z0-9]"), this);
  hostname->setValidator( hostnameValidator );
  
  backend = Backend::instance();
}

void wpNetwork::initializePage()
{
  hostname->setText(backend->cfg("hostname"));
  updateStatus();
}

void wpNetwork::updateStatus()
{
  emit completeChanged();
}

bool wpNetwork::isComplete() const
{
  if(hostname->text().length()) return true;
  return false;
}

bool wpNetwork::validatePage()
{
  if(!isComplete()) return false;
  backend->cfg("hostname", hostname->text());
  return true;
}

