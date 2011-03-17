#include <QObject>
#include <QEvent>

#ifndef BUSYAPPFILTER_H
#define BUSYAPPFILTER_H


class BusyAppFilter : public QObject
{
  protected:
    bool eventFilter( QObject *obj, QEvent *event );
};

#endif // BUSYAPPFILTER_H
