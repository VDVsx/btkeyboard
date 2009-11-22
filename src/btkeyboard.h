/**********************************************************************************
**btkeyboard.h
**
**Copyright (C) 2009 Valerio Valerio <vdv100@gmail.com>
**
**
**This program is free software; you can redistribute it and/or modify
**it under the terms of the GNU General Public License as published by
**the Free Software Foundation; either version 2 of the License, or
**(at your option) any later version.
**
**This program is distributed in the hope that it will be useful,
**but WITHOUT ANY WARRANTY; without even the implied warranty of
**MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**GNU General Public License for more details.
**
**You should have received a copy of the GNU General Public License
**along with this program; if not, write to the Free Software
**Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
**
************************************************************************************/

#include <glib.h>

#include <QString>

#ifndef BTKEYBOARD_H
#define BTKEYBOARD_H


class BtKeyboard
{

public:
    BtKeyboard(QString mode);
    ~BtKeyboard();
    GKeyFile *load_config(const char *file);
    void parseConf(GKeyFile *config);
    bool isDisabled(char **list);
    void disable(GKeyFile *config);
    void enable(GKeyFile *config);

private:
    QList<char*> conf;
    QString mode;
    gsize length;
    gchar** confValues;
    int counter;
    bool isDisabl;


};


#endif // BTKEYBOARD_H
