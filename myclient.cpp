#include <QObject>
#include <QtWidgets>
#include <QtCore>
#include <QtNetwork>
#include "myclient.h"
#include <QApplication>
#include <QDialog>
#include <QInputDialog>
#include <QWidget>
#include <stdlib.h>
#include <QFlags>

MyClient::MyClient(const QString& strHost,
                   int            nPort,
                   QWidget*       pwgt /*=0*/,
                   QString        userNickName
                  ) : QWidget(pwgt)
                 , m_nNextBlockSize(0)

{
    setWindowTitle(userNickName);
    m_pTcpSocket = new QTcpSocket(this);
    m_pTcpSocket->connectToHost(strHost, nPort); //где strHost -
    // имя компьютера или его ip,nPort - номер порта.
    // m_pTcpSocket пытается сам установить связь с сервером, и в
    // случае успеха высылает сигнал connected():

    NickName = userNickName;
    connect(m_pTcpSocket, SIGNAL(connected()), SLOT(slotSendNick()));
//    connect(m_pTcpSocket, SIGNAL(connected()), SLOT(slotConnected()));

    connect(m_pTcpSocket, SIGNAL(readyRead()), SLOT(slotReadyRead()));
    connect(m_pTcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),
            this,         SLOT(slotError(QAbstractSocket::SocketError))
           );

    m_ptxtInfo  = new QTextEdit; // многострочное поле
    m_ptxtInput = new QLineEdit; // однострочное поле
    m_UserList  = new QComboBox; // список ников

    m_ptxtInfo->setReadOnly(true);

    QPushButton* pcmd = new QPushButton("&Send");
    //Message->append(m_ptxtInput->text());
    inputToText =  m_ptxtInput->text();
    // отправка сообщения на сервер при нажатии на кнопку   

    connect(pcmd, SIGNAL(clicked()), SLOT(slotSendToServer()));
    // отправка сообщения на сервер при нажатии Enter
    connect(m_ptxtInput, SIGNAL(returnPressed()),this,SLOT(slotSendToServer()));

    //Layout setup
    QVBoxLayout* pvbxLayout = new QVBoxLayout;
//   pvbxLayout->addWidget(new QLabel(userNickName));
//  pvbxLayout->addWidget(new QLabel("<H1>Client</H1>"));
    pvbxLayout->addWidget(m_ptxtInfo);
    pvbxLayout->addWidget(m_ptxtInput);
    pvbxLayout->addWidget(m_UserList);
    pvbxLayout->addWidget(pcmd);
    setLayout(pvbxLayout);
    m_ptxtInput->setText("");

}

//***********************************************************************
// получение данных от сервера
void MyClient::slotReadyRead()
{
    QDataStream in(m_pTcpSocket);
    in.setVersion(QDataStream::Qt_5_3);
    // с сервера не все данные могут прийти сразу
    // нужно уметь получать данные целиком и по частям или сразу все
    // каждый принятый блок начинается полем размера блока
    for (;;) {

        if (!m_nNextBlockSize) {
            if (m_pTcpSocket->bytesAvailable() < sizeof(quint16)) {
                // sizeof(quint16)) = 2 байта
                // размер полученных данных не меньше двух байтов.
                // bytesAvailable() возвращает число байт,
                // доступных для чтения.
                break;
            }
            // если блок получен целиком то он считывается оператором
            // '>>' объекта потока QDataStream (переменная in)
            in >> m_nNextBlockSize;
        }
        // размер доступных для чтения данных >/= размеру блока
        if (m_pTcpSocket->bytesAvailable() < m_nNextBlockSize) {
            break;
        }
        QTime  time;
        QString str1;
        QString str2;
        in >> time >> str2 >> str1;
        // qDebug() << "2 nickname from client:" << NickName;
        m_ptxtInfo->append(time.toString()+" "+ str2 + str1);
        m_nNextBlockSize = 0;   // размер очередного текстового блока
                                // неизвестен
    }
}
//***********************************************************************

//***********************************************************************

void MyClient::slotError(QAbstractSocket::SocketError err)
{
    QString strError =
        "Error: " + (err == QAbstractSocket::HostNotFoundError ?
                     "The host was not found." :
                     err == QAbstractSocket::RemoteHostClosedError ?
                     "The remote host is closed." :
                     err == QAbstractSocket::ConnectionRefusedError ?
                     "The connection was refused." :
                     QString(m_pTcpSocket->errorString())
                    );
    m_ptxtInfo->append(strError); // текст отображается в виджете
                                  // многострочного текстового поля
}

//***********************************************************************

// метод отсылки запроса клиента серверу

void MyClient::slotSendToServer()
{
    QByteArray  arrBlock; // массив байтов
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_2);
    out << quint16(0) << QTime::currentTime() << m_ptxtInput->text() << NickName ;
    // qDebug() <<  "3 from client nickname:" << NickName;
    out.device()->seek(0);
    out << quint16(arrBlock.size() - sizeof(quint16));
    m_pTcpSocket->write(arrBlock);
    // после отсылки данных мы дополнительно стираем текст в поле
    // ввода сообщения, вызывая метод setText() с пустой строкой
    // в качестве параметра
    m_ptxtInput->setText("");
}

//***********************************************************************

// клиент подсоединился к серверу

void MyClient::slotConnected()
{
    m_ptxtInfo->append("Received the connected() signal");
}

//***********************************************************************

void MyClient::slotSendNick(MyClient::Options flag)
{
    QByteArray  arrBlock; // массив байтов
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_2);
    out << quint16(0) << NickName ;
    out.device()->seek(0);
    out << quint16(arrBlock.size() - sizeof(quint16));
    m_pTcpSocket->write(arrBlock);
    m_ptxtInput->setText("");
    qDebug() << "nick: " << NickName;
}
