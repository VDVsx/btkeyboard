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

#include <glib.h>

#include <QtDebug>
#include <QStringList>

#include "btkeyboard.h"



BtKeyboard::BtKeyboard(QString mode){

    QTextStream out(stdout);

    const char *conf_file = "main.conf";
    char input[] = "input";

    GKeyFile *keyfile;
    GKeyFileFlags flags;
    GError *error = NULL;
    gsize length;
    gchar** confValues;
    gchar* saved_data;
    bool b_ret;
    QList<char*> conf;


    keyfile = g_key_file_new();
    g_key_file_set_list_separator (keyfile,',');
    flags = G_KEY_FILE_KEEP_COMMENTS;

    if (!g_key_file_load_from_file(keyfile, conf_file, flags, &error))
      {
        out << "error: cant find conf file" << endl;
      }


    confValues = g_key_file_get_string_list(keyfile, "General","DisablePlugins", &length, NULL);

    if (length != 0){

        for (gsize i= 0; i < length; i++){
            conf.append(confValues[i]);
        }
    }

    else{

        out << "error: cant find option" << endl;
    }


    //disable

    if (mode == "--disable"){

        if (conf.contains(input)){

            out << "already disabled" << endl;
        }

        else{

            counter = 0;
            conf.append(input);
            memset(confValues, 0, length);

            QListIterator<char*> confIterator(conf);

            while (confIterator.hasNext()){

                    confValues[counter] = confIterator.next();
                    counter++;
            }

            //stop/start bluetoothd
            g_key_file_set_string_list(keyfile, "General","DisablePlugins", confValues, counter);
            out << "updated" << endl;

        }


    }

     //enable

    else{

        out << "no not" << endl;

    }


    saved_data = g_key_file_to_data (keyfile, &length, NULL);

    if (!saved_data){
        out << "error" << endl;

    }

    //Write data to file
    b_ret = g_file_set_contents (conf_file, saved_data, length, NULL);
    g_free (saved_data);

    if (!b_ret){

       out << "error" << endl;

    }

    //free file
    g_key_file_free(keyfile);

    /*
    QSettings settings("main.conf",QSettings::NativeFormat);

    QStringList value = settings.value("DisablePlugins").toStringList();

    value.append("cenas");

    settings.setValue("DisablePlugins",QVariant(value));
    settings.sync();

//QStringList keys = settings.allKeys();

    //qDebug() << keys;
    value = settings.value("DisablePlugins").toStringList();

    qDebug() << value;*/

}
