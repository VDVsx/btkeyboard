#include <glib.h>

#include <QtDebug>
#include <QStringList>

#include "btkeyboard.h"



BtKeyboard::BtKeyboard(QString mode){

    QTextStream out(stdout);

    const char conf_file[] = "main.conf";

    GKeyFile *keyfile;
    GKeyFileFlags flags;
    GError *error = NULL;
    gsize length;
    char** confValues;
    char* saved_data;
    bool b_ret;
    QStringList conf;


    keyfile = g_key_file_new();
    g_key_file_set_list_separator (keyfile,',');
    flags = G_KEY_FILE_KEEP_COMMENTS;

    if (!g_key_file_load_from_file(keyfile, conf_file, flags, &error))
      {
        out << "error" << endl;
      }


    confValues = g_key_file_get_string_list(keyfile, "General","DisablePlugins", &length, NULL);

    if (length != 0){

        for (gsize i= 0; i < length; i++){
            out << confValues[i] << endl;
        }

        confValues[length] = "cenas";
        //length += 1;
        g_key_file_set_string_list(keyfile, "General","DisablePlugins", confValues, length);

        confValues = g_key_file_get_string_list(keyfile, "General","DisablePlugins", &length, NULL);

        if (length != 0){

            for (gsize i= 0; i < length; i++){
                out << confValues[i] << endl;
            }
        }

    }

    else{

        out << "cant find conf file" << endl;
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
