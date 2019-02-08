#include <QApplication>
#include <QtNetwork>
#include <QtWidgets>
#include "myclient.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
    QApplication app(argc, argv);

//    InputDialog* pInputDialog = new InputDialog;

//    if (pInputDialog->exec() == QDialog::Accepted)
//      {
//        MyClient client("localhost", 2323);
//        client.show();
//      }

//    InputDialog* pInputDialog = new InputDialog;

//    if (pInputDialog->exec() == QDialog::Accepted)
//    {
//        QString a = pInputDialog->slotGiveNickName();
    int nPort;
    QString userNickName;
    QString strHost;

    if (argc != 4) {
        //printf(argc);
        printf("Error: found %d arguments. Needs exactly 3", argc-1);
        exit(1);
    }
    nPort = atoi(argv[1]);
    strHost = argv[2];
    userNickName = argv[3];


//    const QString& nick = new const QString;
//    *userNickName = & nick;

    MyClient client(strHost, nPort, 0, userNickName);
        //MyClient client("localhost", 2323,0, a );
       // QString userNickName = pInputDialog->slotGiveNickName();

      //  qDebug() << pInputDialog->slotGiveNickName();
        // qDebug() << "nickname from main client:" << a;
        client.show();


        return app.exec();
    }
