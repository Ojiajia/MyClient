#include <QObject>
#include <QtWidgets>
#include <QtCore>
#include <QtNetwork>
#include "myclient.h"
#include <QApplication>
#include <QDialog>
#include <QInputDialog>
#include <QWidget>

InputDialog::InputDialog(QWidget* pwgt/*= 0*/)
     : QDialog(pwgt, Qt::WindowTitleHint | Qt::WindowSystemMenuHint)

/*   Qt::WindowTitleHint Gives the window a title bar.

     Qt::WindowSystemMenuHint Adds a window system menu,
     and possibly a close button (for example on Mac). If you need
     to hide or show a close button, it is more portable to use
                                     WindowCloseButtonHint.        */
{
    m_ptxtFirstName       = new QLineEdit;
    QLabel* plblFirstName = new QLabel("&Nick Name");

    plblFirstName->setBuddy(m_ptxtFirstName);

    QPushButton* pcmdOk     = new QPushButton("&Ok");
    QPushButton* pcmdCancel = new QPushButton("&Cancel");

    //this->giveNickName();
    connect(pcmdOk,     SIGNAL(clicked()), SLOT(accept()));
//  connect(pcmdOk,     SIGNAL(clicked()), SLOT(slotGiveNickName()));

    connect(pcmdCancel, SIGNAL(clicked()), SLOT(reject()));

    //Layout setup

    QGridLayout* ptopLayout = new QGridLayout;
    ptopLayout->addWidget(plblFirstName, 0, 0);
    ptopLayout->addWidget(m_ptxtFirstName, 0, 1);
    ptopLayout->addWidget(pcmdOk, 2,0);
    ptopLayout->addWidget(pcmdCancel, 2, 1);
    setLayout(ptopLayout);
}

QString InputDialog::slotGiveNickName()
{
    return m_ptxtFirstName->text();
}
