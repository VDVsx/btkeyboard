/**********************************************************************************
**btkeyboard.cpp
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

#include <QtDBus>
#include <QVariant>
#include <QtDebug>

#include "btkeyboard.h"

BtKeyboard::BtKeyboard(QString mod)
{
    mode = mod;
}

GKeyFile* BtKeyboard::load_config(const char *file)
{
     GKeyFile *keyfile;
     GKeyFileFlags flags;
     GError *error = NULL;

     keyfile = g_key_file_new();
     g_key_file_set_list_separator (keyfile,',');
     flags = G_KEY_FILE_KEEP_COMMENTS;

     if (!g_key_file_load_from_file(keyfile, file, flags, &error)){
         showInformationNote("Error: Cannot find the configurations file");
         return NULL;
     }

     return keyfile;
}

void BtKeyboard::parseConf(GKeyFile *config)
{
    confValues = g_key_file_get_string_list(config, "General","DisablePlugins", &length, NULL);
}

bool BtKeyboard::isDisabled(char **list)
{
    //returns true if the support is disable, ie the
    //plugin is on the list.
    for (int i = 0; list[i] != NULL; i++) {
        if (g_str_equal("input", list[i]))
            return true;
    }

    return false;
}

void BtKeyboard::saveConfig(GKeyFile *config)
{
    QTextStream out(stdout); //remove

    gchar* saved_data;
    bool b_ret;

    saved_data = g_key_file_to_data (config, &length, NULL);

    if (!saved_data)
        showInformationNote("Error: Cannot save the configurations");

    //Write data to file
    b_ret = g_file_set_contents ("main.conf", saved_data, length, NULL);
    g_free (saved_data);

    if (!b_ret)
       showInformationNote("Error: Cannot save the configurations");

    //free file
    g_key_file_free(config);

}

void BtKeyboard::disable(GKeyFile *config)
{
    parseConf(config);

    if (confValues == NULL){
        error =  "Error: The configurations file is in bad state";
        error += "\nSupport for Bluetooth keyboards disabled";
        showInformationNote(error);
    }

    else{
        disabled = isDisabled(confValues);
        if (disabled)
            showInformationNote("Support for Bluetooth keyboards disabled");

        else{
            confValues[length] = input;
            g_key_file_set_string_list(config, "General","DisablePlugins", confValues, length+1);
            saveConfig(config);
            showInformationNote("Support for Bluetooth keyboards disabled");
        }

    }

}

int BtKeyboard::showInformationNote(QString info_type){
    QString credits = "\n\nCredits:\n\nProgramming: Valerio Valerio <vdv100@gmail.com\n";
    credits += "Application icon: Andrew Zhilin <drew.zhilin@gmail.com>\n";

    QDBusMessage msg = QDBusMessage::createMethodCall("org.freedesktop.Notifications",
                                                      "/org/freedesktop/Notifications",
                                                      "org.freedesktop.Notifications",
                                                      "SystemNoteDialog");
    QString text = info_type + credits;

    QList<QVariant> args;
    args.append(text);
    args.append(static_cast<quint32>(0));
    args.append("ok");

    msg.setArguments(args);

    QDBusConnection::systemBus().call(msg);

    return 1;
}

void BtKeyboard::enable(GKeyFile *config)
{
    parseConf(config);
}

BtKeyboard::~BtKeyboard()
{
}

