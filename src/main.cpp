#include <QTextStream>
#include "btkeyboard.h"


int main(int argc, char *argv[])
 {
    QString arg = argv[1];
    QTextStream out(stdout);

    if (argc <= 1 || arg == "--help" ){

        out << "Usage:" << endl;
        out << "  btkeyboard [OPTION]" << endl << endl;
        out << "Help Option:" << endl;
        out << "  --help        Show help options" << endl << endl;
        out << "Application Options:" << endl;
        out << "  --enable      Enables the support for Bluetooth keyboards" << endl;
        out << "  --disable     Disables the support for Bluetooth keyboards"  << endl << endl;

    }

    else{

        BtKeyboard key(arg);

    }

 }
