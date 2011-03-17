#include "backend.h"
#include <QDebug>
#include <QTime>
#include <stdlib.h>
#include "config.h"

Backend* Backend::_instance;

Backend::Backend()
{
  qDebug("Backend::Backend() called!");
  _process = new QProcess();
  
  QTime time = QTime::currentTime();
  srand((uint)time.msec());
  connect(_process, SIGNAL(finished(int)), this, SLOT(slotProcessExited()));
  connect(_process, SIGNAL(readyReadStandardOutput()), this, SLOT(slotReceiveOutput()));
}

Backend::~Backend()
{
}

void Backend::runBackend()
{
  QStringList command = QStringList() << BACKEND_PATH << "-ni";
  _process->start(command.join(" "));
  busy = true; emit isBusy(true);
}

bool Backend::isBusy()
{
  return (busy && commandQueue.isEmpty());
}

bool Backend::flag(QString flag)
{
  return flags.contains(flag);
}

void Backend::_flag(QString flag, bool set)
{
  if(set)
  {
    if(!flags.contains(flag))
      flags.append(flag);
  }
  else
  {
    if(flags.contains(flag))
      flags.remove(flags.indexOf(flag));
  }
}

void Backend::flag(QString flag, bool set)
{
  if(set)
  {
    exec("flag_set '"+flag.replace("'","\\'")+"'");
    _flag(flag, true);
  }
  else
  {
    exec("flag_unset '"+flag.replace("'","\\'")+"'");
    _flag(flag, false);
  }
}

QString Backend::cfg(QString var)
{
  return cfgMap.value(var);
}

void Backend::_cfg(QString var, QString value)
{
  cfgMap[var] = value;
}

void Backend::cfg(QString var, QString value)
{
  exec("cfg_set '"+var.replace("'","\\'")+"' '"+value.replace("'","\\'")+"'");
  _cfg(var, value);
}

void Backend::exec(QString command)
{
  qDebug("Backend::exec(\""+command.toAscii()+"\") called!");
  commandQueue.enqueue(command);
  dequeue();
}

void Backend::dequeue()
{
  if(!busy && commandQueue.isEmpty()) emit isBusy(false);
  if(busy) return;
  if(commandQueue.isEmpty()) return;
  busy = true; emit isBusy(true);
  QString command = commandQueue.dequeue();
  currentCommand = command;
  data.clear();
  _process->write(command.append('\004').toLocal8Bit());
}

void Backend::exitBackend()
{
  qDebug("Backend::exitBackend() called!");
  _process->kill();
  _process->waitForFinished();
}

void Backend::processOutput(QString line)
{
  qDebug("Backend: input: "+line.toAscii());
  if(line.startsWith("<acritoxinstaller "))
  {
    QString command = line.section(" ",1,1).section(">",0,0);
    QString args = line.section(" ",2).trimmed();
    if(command == "prompt")
    {
      busy = false;
      qDebug("Backend: finished: "+currentCommand.toAscii());
      emit finishedCommand(currentCommand);
      dequeue();
    }
    else if(command == "flag")
    {
      QString setunset = args.section(" ",0,0);
      QString flag = args.section(" ",1).trimmed();
      if(setunset == "set")
	_flag(flag, true);
      else if(setunset == "unset")
	_flag(flag, false);
    }
    else if(command == "data")
    {
      currentData = args.trimmed();
      data[currentData] = "";
    }
    else if(command == "cfg")
    {
      _cfg(args.trimmed(), data["value"].trimmed());
    }
    else if(command == "progress")
    {
      emit receivedProgress(args.toInt());
    }
    else
      emit receivedCommand(command, args);
  }
  else
  {
    data[currentData].append(line);
    emit receivedDataLine(currentData, line.trimmed());
    qDebug("Backend: data: "+line.toAscii());
  }
}

QString Backend::encryptPassword(QString password)
{
  unsigned long seed[2];
  char salt[] = "$1$........";
  char *pwd = password.toAscii().data();
  const char *const seedchars =
    "./0123456789ABCDEFGHIJKLMNOPQRST"
    "UVWXYZabcdefghijklmnopqrstuvwxyz";
  int i;
  
  seed[0] = rand(); seed[1] = rand();
  
  for (i = 0; i < 8; i++)
    salt[3+i] = seedchars[(seed[i/5] >> (i%5)*6) & 0x3f];
    
  return(QString(crypt(pwd, salt)));
}

// function adapted from amarok 2.0 / App.cpp - (C) 2002 by Mark Kretschmann <markey@web.de>
QString Backend::cleanUsername(const QString &username)
{
  QString result = username;
  
  // german umlauts
  result.replace( QChar(0x00e4), "ae" ).replace( QChar(0x00c4), "Ae" );
  result.replace( QChar(0x00f6), "oe" ).replace( QChar(0x00d6), "Oe" );
  result.replace( QChar(0x00fc), "ue" ).replace( QChar(0x00dc), "Ue" );
  result.replace( QChar(0x00df), "ss" );

  // some strange accents
  result.replace( QChar(0x00e7), "c" ).replace( QChar(0x00c7), "C" );
  result.replace( QChar(0x00fd), "y" ).replace( QChar(0x00dd), "Y" );
  result.replace( QChar(0x00f1), "n" ).replace( QChar(0x00d1), "N" );

  // czech letters with carons
  result.replace( QChar(0x0161), "s" ).replace( QChar(0x0160), "S" );
  result.replace( QChar(0x010d), "c" ).replace( QChar(0x010c), "C" );
  result.replace( QChar(0x0159), "r" ).replace( QChar(0x0158), "R" );
  result.replace( QChar(0x017e), "z" ).replace( QChar(0x017d), "Z" );
  result.replace( QChar(0x0165), "t" ).replace( QChar(0x0164), "T" );
  result.replace( QChar(0x0148), "n" ).replace( QChar(0x0147), "N" );
  result.replace( QChar(0x010f), "d" ).replace( QChar(0x010e), "D" );

  // accented vowels
  QChar a[] = { 'a', 0xe0,0xe1,0xe2,0xe3,0xe5, 0 };
  QChar A[] = { 'A', 0xc0,0xc1,0xc2,0xc3,0xc5, 0 };
  QChar E[] = { 'e', 0xe8,0xe9,0xea,0xeb,0x11a, 0 };
  QChar e[] = { 'E', 0xc8,0xc9,0xca,0xcb,0x11b, 0 };
  QChar i[] = { 'i', 0xec,0xed,0xee,0xef, 0 };
  QChar I[] = { 'I', 0xcc,0xcd,0xce,0xcf, 0 };
  QChar o[] = { 'o', 0xf2,0xf3,0xf4,0xf5,0xf8, 0 };
  QChar O[] = { 'O', 0xd2,0xd3,0xd4,0xd5,0xd8, 0 };
  QChar u[] = { 'u', 0xf9,0xfa,0xfb,0x16e, 0 };
  QChar U[] = { 'U', 0xd9,0xda,0xdb,0x16f, 0 };
  QChar nul[] = { 0 };
  QChar *replacements[] = { a, A, e, E, i, I, o, O, u, U, nul };

  for( int i = 0; i < result.length(); i++ )
  {
    QChar c = result[ i ];
    for( uint n = 0; replacements[n][0] != QChar(0); n++ )
    {
      for( uint k=0; replacements[n][k] != QChar(0); k++ )
      {
        if( replacements[n][k] == c )
        {
          c = replacements[n][0];
        }
      }
    }
    result[ i ] = c;
  }
  
  return result.replace(QRegExp("[^a-zA-Z0-9-_.]"), "");
}

void Backend::slotProcessExited()
{
  emit processExited();
}

void Backend::slotReceiveOutput()
{
  _process->setReadChannel(QProcess::StandardOutput);
  char data[2048]; QString line;
  while(_process->canReadLine())
  {
    int result = _process->readLine(data, sizeof(data));
    if(result == -1) break;
    line = QString::fromLocal8Bit(data);
    processOutput(line);
  }
}
