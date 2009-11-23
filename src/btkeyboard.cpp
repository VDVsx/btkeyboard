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

#include "btkeyboard.h"

BtKeyboard::BtKeyboard(QString mod)
{
    mode = mod;
    input = "input";
}

GKeyFile* BtKeyboard::load_config(const char *file)
{
     GKeyFile *keyfile;
     GKeyFileFlags flags;
     GError *error = NULL;

     keyfile = g_key_file_new();
     g_key_file_set_list_separator (keyfile,',');
     flags = G_KEY_FILE_KEEP_COMMENTS;

     if (!g_key_file_load_from_file(keyfile, file, flags, &error)) {
         showInformationNote("Error: Cannot find configuration file");
         return NULL;
     }

     return keyfile;
}

void BtKeyboard::parseConf(GKeyFile *config)
{
    confValues = g_key_file_get_string_list(config, "General","DisablePlugins", &length, NULL);

    if (confValues == NULL) {
        error =  "Error: Can't parse configuration file";
        error += "\nSupport for Bluetooth keyboard disabled";
        showInformationNote(error);
    }
}

bool BtKeyboard::isDisabled(char **list)
{
    //returns true if the support is disabled, ie the
    //plugin is on the list.
    for (int i = 0; list[i] != NULL; i++) {
        if (g_str_equal("input", list[i]))
            return true;
    }

    return false;
}

void BtKeyboard::saveConfig(GKeyFile *config)
{
    gchar* saved_data;
    bool b_ret;

    saved_data = g_key_file_to_data (config, &length, NULL);

    if (!saved_data)
        showInformationNote("Error: Cannot save settings");

    //Write data to file
    b_ret = g_file_set_contents ("/etc/bluetooth/main.conf", saved_data, length, NULL);
    g_free (saved_data);

    if (!b_ret)
       showInformationNote("Error: Cannot save settings");

    //free file
    g_key_file_free(config);

}

void BtKeyboard::disable(GKeyFile *config)
{
    parseConf(config);
    disabled = isDisabled(confValues);

    if (disabled)
        showInformationNote("Support for Bluetooth keyboard disabled");

    else{
        confValues[length] = input;
        g_key_file_set_string_list(config, "General","DisablePlugins", confValues, length+1);
        saveConfig(config);

        int tmp = system("stop bluetoothd");

        //should never happen
        if (tmp < 0) {
            error = "Error: Cannot restart the Bluetooth system";
            error += "\nPlease restart your device to enable the support for Bluetooth keyboard";
            showInformationNote(error);
        }

        tmp = system("start bluetoothd");

        //should never happen
        if (tmp < 0) {
            error = "Error: Cannot start the Bluetooth system";
            error += "\nPlease restart your device";
            showInformationNote(error);
        }

        showInformationNote("Support for Bluetooth keyboard disabled");
    }

}

void BtKeyboard::enable(GKeyFile *config)
{
    parseConf(config);
    disabled = isDisabled(confValues);

    if (disabled) {
        counter = 0;
        gchar *confValuesAux[length-1];

        for (int i = 0; confValues[i] != NULL; i++) {
            if (!g_str_equal("input", confValues[i])) {
                confValuesAux[counter] = confValues[i];
                counter++;
            }
        }

        g_key_file_set_string_list(config, "General","DisablePlugins", confValuesAux, counter);
        saveConfig(config);

        int tmp = system("stop bluetoothd");

        //should never happen
        if (tmp < 0){
            error = "Error: Cannot restart the Bluetooth system";
            error += "\nPlease restart your device to enable the support for Bluetooth keyboard";
            showInformationNote(error);
        }

        tmp = system("start bluetoothd");

        //should never happen
        if (tmp < 0) {
            error = "Error: Cannot start the Bluetooth system";
            error += "\nPlease restart your device";
            showInformationNote(error);
        }

        showInformationNote("Support for Bluetooth keyboard enabled");
    }

    else
        showInformationNote("Support for Bluetooth keyboard enabled");
}

void BtKeyboard::state(GKeyFile *config)
{
    parseConf(config);
    disabled = isDisabled(confValues);

    if(disabled)
        showInformationNote("Support for Bluetooth keyboard disabled");
    else
        showInformationNote("Support for Bluetooth keyboard enabled");

}

void BtKeyboard::showInformationNote(QString info_type){
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

    exit(1);
}

BtKeyboard::~BtKeyboard()
{
}
