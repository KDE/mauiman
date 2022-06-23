#include "server.h"

int main(int argc, char *argv[])
{
    Server s(argc, argv);
//    s.setQuitOnLastWindowClosed(false);
    if(!s.init())
    {
        QCoreApplication::exit(-1);
        return -1;
    }

    return s.exec();
}
