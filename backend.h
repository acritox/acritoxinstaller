#include <QProcess>
#include <QQueue>
#include <QMap>
#include <QVector>

#ifndef BACKEND_H
#define BACKEND_H

class Backend : public QObject
{
    Q_OBJECT

public:
    static Backend* instance() {if(!_instance) _instance = new Backend(); return(_instance);}
    ~Backend();
    void init();
    void runBackend();
    void exec(QString command);
    void exitBackend();
    QString encryptPassword(QString password);
    QString cleanUsername(const QString &username);
    bool isBusy();
    bool flag(QString flag);
    void flag(QString flag, bool set);
    QString cfg(QString var);
    void cfg(QString var, QString value);
      
protected:
    QProcess* _process;

private slots:
    void slotProcessExited();
    void slotReceiveOutput();

private:
    static Backend *_instance;
    Backend();
    void processOutput(QString line);
    void _flag(QString flag, bool set);
    void _cfg(QString var, QString value);
    void dequeue();
    QMap<QString,QString> data;
    QString currentData;
    QMap<QString,QString> cfgMap;
    QVector<QString> flags;
    QQueue<QString> commandQueue;
    QString currentCommand;
    bool busy;
    
signals:
    void processExited();
    void receivedDataLine(QString data, QString line);
    void receivedProgress(int percent);
    void receivedCommand(QString command, QString args);
    void finishedCommand(QString command);
    void isBusy(bool busy);
};

#endif
