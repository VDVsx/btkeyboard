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

#include <QtDebug>
#include <QStringList>

#include "btkeyboard.h"

BtKeyboard::BtKeyboard(QString mod)
{
    mode = mod;
    QTextStream out(stdout);

    //const char *conf_file = "main.conf";
    char input[] = "input";


/*
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
    g_key_file_free(keyfile);*/


}

GKeyFile* BtKeyboard::load_config(const char *file)
{
     GKeyFile *keyfile;
     GKeyFileFlags flags;
     GError *error = NULL;
     gchar* saved_data;
     bool b_ret;

     QTextStream out(stdout); //remove

     keyfile = g_key_file_new();
     g_key_file_set_list_separator (keyfile,',');
     flags = G_KEY_FILE_KEEP_COMMENTS;

     if (!g_key_file_load_from_file(keyfile, file, flags, &error)){
         out << "error: cant find conf file" << endl;
         return NULL;
     }

     return keyfile;
}

void BtKeyboard::parseConf(GKeyFile *config)
{
    confValues = g_key_file_get_string_list(config, "General","DisablePlugins", &length, NULL);

    //return confValues;

     // por isto no outro lado -->   out << "error: cant find option" << endl;
}

bool BtKeyboard::isDisabled(char **list)
{
    //returns true if the support is disable, ie the
    //plugin is on the list.

    for (int i = 0; list[i] != NULL; i++) {
        bool equal;

        if (g_str_equal("input", list[i]))
            return true;
    }

    return false;
}


void BtKeyboard::disable(GKeyFile *config)
{
    QTextStream out(stdout); //remove

    parseConf(config);

    if (confValues == NULL){
        out << "error: cant find option" << endl;
    }

    else{
        isDisabl = isDisabled(confValues);
        if (isDisabl){

            out << "KB Bluetooth Support disabled" << endl;
        }

        else{


        }

    }


}

void BtKeyboard::enable(GKeyFile *config)
{
    parseConf(config);
}

BtKeyboard::~BtKeyboard()
{
}

