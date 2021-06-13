#include <ActionsBar.h>
#include <QApplication>
#include <SalActIcon.h>
#include <QDebug>
#include <QPushButton>
#include <MoveControl.h>
#include <QString>
#include <QSystemTrayIcon>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QLabel* testLabel = new QLabel("testaaa");
    testLabel->setGeometry(300,300,200,100);
    ActionsBar *act = new ActionsBar();
    auto mcrtl = act->addMoveControl(testLabel);
    act->addButton(SalActIcon::PasteImage()); 
    auto btnHide = act->addButton(SalActIcon::Pencil());
    QObject::connect(btnHide, &QPushButton::clicked, [&](){
        act->hideToTray(true);
    });

    act->addButton(SalActIcon::Keyboard());
    act->addGapLine();
    act->addLabel("Secondary Mode");
    auto closeBtn = act->addButton(SalActIcon::Close());
    act->setTrayIcon(SalActIcon::Arrow());
    
    ActionsBar *act2 = new ActionsBar();
    act2->addButton(SalActIcon::PasteImage());
    act2->addButton(SalActIcon::Pencil());
    act2->addButton(SalActIcon::Keyboard());
    act2->addLabel("Secondary Mode");
    act2->addLabel("Secondary Mode");
    act2->addLabel("Secondary Mode");
    act->addLowWidget(act2);
    QObject::connect(act, &ActionsBar::sigMove, [&](const QPoint &pos){
        testLabel->move(pos);
        act->followAdjust(testLabel->geometry());
    });
    QObject::connect(closeBtn, &QPushButton::clicked, testLabel, &QLabel::deleteLater);
    QObject::connect(closeBtn, &QPushButton::clicked, &a, &QApplication::exit);
    act->show();
    testLabel->show();
    act->followAdjust(testLabel->geometry());
    return a.exec();
}