#pragma once
#include "salmonactions_global.h"
#include <qwidget.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <MoveControl.h>
#include <QDialog>
#include <QAction>
#include <QSystemTrayIcon>
class QHBoxLayout;
class QVBoxLayout;
class SALMONACTIONS_EXPORT ActionsBar :
    public QWidget
{
    Q_OBJECT
public:
    ActionsBar(QWidget* parent = nullptr);
    ~ActionsBar();
    void addGapLine();
    QPushButton* addButton(const QIcon& icon);
    QPushButton* addButton(const QIcon& icon, const QString &helpStr);
    MoveControl* addMoveControl(QWidget* bindWidget);
    QLabel* addLabel(QString text = QString(), int width = -1);
    void addLowWidget(ActionsBar* lowWidget);
    void deleteLowWidget(ActionsBar* lowWidget);
    void adjustLowWidget();
    void closeLowWidget();
    void setGap(int gap) { mGap = gap; };
    bool hasMouse() { return mHasMouse; };
    void enterEvent(QEvent* event);
    void leaveEvent(QEvent* event);
    void followAdjust(const QRect& mainRect);
    void readjustWidth();
    void reAdjustGeo();
    QList<QWidget *> *actionList(){return &mActionList;};
    void hideToTray(bool enable);
    void setTrayIcon(QIcon icon);
    void staysOnTop(bool enable);
    QSystemTrayIcon *systemTrayIcon() {return mSystemTrayIcon;};
signals:
    void sigMouseHover(bool hover);
    void sigMove(const QPoint& pos);
    void SigStaysOnTopChanged(bool enable);
    void SigWindowDeactivate();
    void SigWindowActivate();
    void SigTrayIconStateChange(bool isHideToTray);
protected:
    virtual void paintEvent(QPaintEvent *event);
    virtual void closeEvent(QCloseEvent *event);
    virtual void changeEvent(QEvent *event);
    virtual bool event(QEvent *event);
private:
    void initSystemTray();
    void onTrayActivated(QSystemTrayIcon::ActivationReason reason);
    bool mHasMouse = false;
    QList<QWidget *> mActionList;
    QList<ActionsBar *> mLowBarList;
    int mMainBarWidth = 0;
    QSize mBtnSize;
    int mGap = 8;
    int mBoardWidth = 1;
    QWidget* mLowWidget = nullptr;
    bool showOnTop = false;
    QSystemTrayIcon *mSystemTrayIcon = nullptr;
    QIcon mTrayIcon;
    MoveControl *mMoveBtn = nullptr;
    QRect mMainRect;
    bool mStaysOnTop = true;
};
