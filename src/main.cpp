/**********************************************************************************
**main.cpp
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

#include <QTextStream>
#include "btkeyboard.h"

int main(int argc, char *argv[])
 {
    QString arg = argv[1];
    QTextStream out(stdout);
    GKeyFile *config;

    if (arg == "--disable" || arg == "--enable" || arg == "--state") {
        BtKeyboard btkb(arg);

        config = btkb.load_config("/etc/bluetooth/main.conf");

        if (arg == "--disable")
            btkb.disable(config);
        else if (arg == "--enable")
            btkb.enable(config);
        else
            btkb.state(config);
    }

    else{
        out << "Usage:" << endl;
        out << "  btkeyboard [OPTION]" << endl << endl;
        out << "Help Option:" << endl;
        out << "  --help        Show help options" << endl << endl;
        out << "Application Options:" << endl;
        out << "  --enable      Enables the support for Bluetooth keyboard" << endl;
        out << "  --disable     Disables the support for Bluetooth keyboard"  << endl;
        out << "  --state       Shows current state(Enabled|Disabled)" << endl << endl;
    }
 }
