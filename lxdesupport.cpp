/***************************************************************************
 *   Copyright (C) 2010 by Ilya Kotov                                      *
 *   forkotov02@hotmail.ru                                                 *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include <QIcon>
#include <QSettings>
#include <QDir>
#include "lxdesupport.h"

void LXDESupport::load()
{
    QIcon::setThemeName("acritoxinstaller"); // use kanotix theme
    if(qgetenv("XDG_CURRENT_DESKTOP") != "LXDE")
        return;

    QSettings lxde_settings("lxsession/LXDE", "desktop");
    QString themeName = lxde_settings.value("GTK/sNet/IconThemeName").toString();
    if(!themeName.isEmpty())
        QIcon::setThemeName("acritoxinstaller"); // also use kanotix theme for LXDE
}

