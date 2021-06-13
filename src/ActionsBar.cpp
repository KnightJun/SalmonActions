#include "ActionsBar.h"
#include <QPushButton>
#include <QHBoxLayout>
#include <QDesktopWidget>
#include <QApplication>
#include <MoveControl.h>
#include <QPainter>
#include <qdebug.h>
#include <QEvent>
#include <QCloseEvent>
#include <QWindow>
#include <QScreen>
ActionsBar::ActionsBar(QWidget* parent):QWidget(parent)
{
	this->setStyleSheet("QWidget{\nbackground: rgb(255, 255, 255);\ncolor: rgb(48, 48, 48);\n}\nQPushButton{  \nborder:0px;  \nbackground: rgb(255, 255, 255);  \npadding:8px 8px 8px 8px;\n}  \nQPushButton:hover{  /*�����Ϻ�*/  \nbackground: rgb(229, 243, 255);  \n}\nQPushButton:pressed{ /*���°�ť��*/  \nbackground: rgb(205, 232, 255);   \n}\nQPushButton:checked{\nbackground: rgb(205, 232, 255);   \n}\nQLabel{  \nbackground: rgb(255, 255, 255);  \n}\nQFrame{\nbackground: rgb(255, 255, 255);  \n}");
    this->setFont(QFont("Microsoft YaHei UI", 12));
    mBtnSize = QSize(36, 36);
    this->resize(mBoardWidth * 2, mBtnSize.height() + mBoardWidth * 2);
    this->setWindowFlags(Qt::Dialog | Qt::Tool | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
}

void ActionsBar::staysOnTop(bool enable)
{
    if(enable == mStaysOnTop)return;
    mStaysOnTop = enable;
    this->setWindowFlag(Qt::WindowStaysOnTopHint, enable);
    if(!mSystemTrayIcon){
        this->show();
    }
    emit SigStaysOnTopChanged(enable);
}

bool ActionsBar::event(QEvent *event)
{
    if(QEvent::WindowDeactivate == event->type())
    {
        emit SigWindowDeactivate();
    }else if (QEvent::WindowActivate == event->type()){
        emit SigWindowActivate();
    }
    return QWidget::event(event);
}

QPushButton* ActionsBar::addButton(const QIcon& icon)
{
    return addButton(icon, "");
}

void ActionsBar::paintEvent(QPaintEvent *event)
{
    //QWidget::paintEvent(event);
    QPainter painter(this);
    painter.setPen(qRgb(30,30,30));
    painter.setBrush(Qt::white);
    painter.drawRect(0, 0, 
    mMainBarWidth - 1, 
    mBtnSize.height() + mBoardWidth * 2 - 1);
}

void ActionsBar::addGapLine()
{
    QFrame* line = new QFrame(this);
    line->setObjectName(QString::fromUtf8("line"));
    line->setStyleSheet("QFrame{\ncolor: rgb(130, 130, 130);\n}");
    line->setFrameShape(QFrame::VLine);
    line->setFrameShadow(QFrame::Plain);
    line->setFixedHeight(mBtnSize.height() * 0.618);
    line->setFixedWidth(3);
    mActionList.append(line);
	readjustWidth();
}

QPushButton* ActionsBar::addButton(const QIcon& icon, const QString& helpStr)
{
    QPushButton* toolBtn = new QPushButton(this);
    toolBtn->setIcon(icon);
    toolBtn->setIconSize(QSize(20, 20));
    toolBtn->setFixedSize(mBtnSize);
    toolBtn->setFocusPolicy(Qt::NoFocus);
    mActionList.append(toolBtn);
	readjustWidth();
    return toolBtn;
}
void ActionsBar::setTrayIcon(QIcon icon)
{
    mTrayIcon = icon;
    if(mSystemTrayIcon){
        mSystemTrayIcon->setIcon(mTrayIcon);
    }
}

void ActionsBar::initSystemTray()
{
    if(mSystemTrayIcon == nullptr){
        mSystemTrayIcon = new QSystemTrayIcon(mTrayIcon, this);
        connect(mSystemTrayIcon, &QSystemTrayIcon::activated, this, &ActionsBar::onTrayActivated);
    }
}

void ActionsBar::onTrayActivated(QSystemTrayIcon::ActivationReason reason)
{
    QRect geo = mSystemTrayIcon->geometry();
    QDesktopWidget* desktopWidget = QApplication::desktop();
    QRect deskRect = desktopWidget->availableGeometry();
    QRect screenRect = desktopWidget->screenGeometry();
    QPoint showPoint;

    if (screenRect.bottom() != deskRect.bottom()) //taskbar on bottom
    {
        showPoint.setX(geo.x() + geo.width() / 2 - this->width() / 2);
        showPoint.setY(geo.top() - this->height());
    }
    else if (screenRect.top() != deskRect.top()) //taskbar on top
    {
        showPoint.setX(geo.x() + geo.width() / 2 - this->width() / 2);
        showPoint.setY(geo.bottom());
    }
    else if (screenRect.left() != deskRect.left()) //left
    {
        showPoint.setY(geo.y() + geo.height() / 2 - this->height() / 2);
        showPoint.setX(geo.right());
    }
    else if (screenRect.right() != deskRect.right()) //right
    {
        showPoint.setY(geo.y() + geo.height() / 2 - this->height() / 2);
        showPoint.setX(geo.left() - this->width());
    }
    else    //hide
    {
        qDebug() << "taskbar hide!";
        showPoint.setX(geo.x() + geo.width() / 2 - this->width() / 2);
        showPoint.setY(geo.top() - this->height());
    }
    // widget limit in screen
    if(showPoint.x() < 0) showPoint.setX(0);
    if(showPoint.x() + this->width() > screenRect.right()){
        showPoint.setX(screenRect.right() - this->width());
    }
    if(showPoint.y() < 0) showPoint.setY(0);
    if(showPoint.y() + this->height() > screenRect.bottom()){
        showPoint.setY(screenRect.bottom() - this->height());
    }
    this->move(showPoint);
    this->show();
    this->raise();
    this->activateWindow();
}

void ActionsBar::changeEvent(QEvent *event)
{    
    QWidget::changeEvent(event);
        
    if (event->type() == QEvent::ActivationChange)
    {
        if(mSystemTrayIcon && mSystemTrayIcon->isVisible() && !this->isActiveWindow()){
            this->hide();
        }
    }
}

void ActionsBar::hideToTray(bool enable)
{
    if(enable){
        if(mSystemTrayIcon)return;
        initSystemTray();
        mSystemTrayIcon->show();
        this->hide();
    }else{
        if(!mSystemTrayIcon)return;
        mSystemTrayIcon->deleteLater();
        this->show();
        mSystemTrayIcon = nullptr;
    }
    emit SigTrayIconStateChange(enable);
}

void ActionsBar::reAdjustGeo()
{
    QRect newGeo;
    newGeo.setX(this->x());
    newGeo.setY(this->y());
    newGeo.setHeight(mBtnSize.height() + mBoardWidth * 2);
    newGeo.setWidth(mMainBarWidth);
    int newBarTop = newGeo.height() - mBoardWidth;
    for (ActionsBar *actbar : mLowBarList)
    {
        int barWidth = actbar->mMainBarWidth;
        int barHeight = actbar->mBtnSize.height() + mBoardWidth * 2;
        actbar->setGeometry(0, newBarTop, 
            barWidth, barHeight);
        newGeo.setHeight(newGeo.height() + barHeight - mBoardWidth);
        if(barWidth > newGeo.width()){
            newGeo.setWidth(barWidth);
        }
        newBarTop += actbar->height() - mBoardWidth;
    }
    this->setGeometry(newGeo);
}

void ActionsBar::deleteLowWidget(ActionsBar* lowWidget)
{
    mLowBarList.removeOne(lowWidget);
    lowWidget->deleteLater();
    this->reAdjustGeo();
}

void ActionsBar::addLowWidget(ActionsBar* lowWidget)
{
    lowWidget->setParent(this);
    mLowBarList.append(lowWidget);
    lowWidget->setGeometry(0, mBtnSize.height() + mBoardWidth * 2 - 1, lowWidget->width(), lowWidget->height());

    this->reAdjustGeo();
    lowWidget->setVisible(true);
    return;
}

void ActionsBar::adjustLowWidget()
{
    return;
    if(showOnTop){
        mLowWidget->move(this->x(), this->y() - mLowWidget->height() + 1);
    }else{
        mLowWidget->move(this->x(), this->y() + mLowWidget->height() - 1);
    }
}

void ActionsBar::closeLowWidget()
{
    if (mLowWidget != sender())return;
    mLowWidget = nullptr;
}

MoveControl* ActionsBar::addMoveControl(QWidget* bindWidget)
{
    mMoveBtn = new MoveControl(bindWidget, this);
    mMoveBtn->setIconSize(QSize(20, 20));
    mMoveBtn->setFixedSize(mBtnSize);
    mMoveBtn->setFocusPolicy(Qt::NoFocus);
    connect(mMoveBtn, &MoveControl::sigMove, this, &ActionsBar::sigMove);
    connect(mMoveBtn, &MoveControl::sigMoveFinish, [&](){
        followAdjust(mMainRect);
    });
    mActionList.append(mMoveBtn);
    readjustWidth();
    return mMoveBtn;
}

void ActionsBar::closeEvent(QCloseEvent *event)
{
    event->ignore();
}

void ActionsBar::readjustWidth()
{
    int srcWidth = this->width();
    int dstWidth = 0;
    for(auto actWid : mActionList){
        if(actWid->isVisible() || !this->isVisible() ){
            actWid->move(mBoardWidth + dstWidth, 
                (mBtnSize.height() - actWid->height()) / 2 + mBoardWidth);
            dstWidth += actWid->width();
        }
    }
    mMainBarWidth = dstWidth += mBoardWidth * 2;
    for (ActionsBar *actbar : mLowBarList)
    {
        if(actbar->mMainBarWidth > dstWidth){
            dstWidth = actbar->mMainBarWidth;
        }
    }
    this->setGeometry(this->x() + (srcWidth - dstWidth), this->y()
        , dstWidth, this->height());
}

QLabel* ActionsBar::addLabel(QString text, int width)
{
    QLabel* lbl = new QLabel(this);
    lbl->setAlignment(Qt::AlignCenter);
    lbl->setFont(this->font());
    if(width <= 0){
        width = this->fontMetrics().width(text) + 20;
    }
    lbl->setFixedWidth(width);
    lbl->setText(text);
    lbl->setFixedHeight(mBtnSize.height());
    mActionList.append(lbl);
    readjustWidth();
    return lbl;
}

void ActionsBar::enterEvent(QEvent* event)
{
    mHasMouse = true;
    emit sigMouseHover(true);
}

void ActionsBar::leaveEvent(QEvent* event)
{
    mHasMouse = false;
    emit sigMouseHover(false);
}

void ActionsBar::followAdjust(const QRect& mainRect)
{
    mMainRect = mainRect;
    QPoint pos = QWidget::mapToGlobal(mainRect.topLeft());
    QRect deskRt = this->windowHandle()->screen()->availableGeometry();
    int mWidHeight = this->height() + (mLowWidget ? mLowWidget->height():0);
    int posX = mainRect.right() - mMainBarWidth, posY = mainRect.bottom() + mGap;
    if(mMoveBtn && mMoveBtn->isMoving()){
        if(showOnTop){
            posY = mainRect.y() - this->height() - mGap;
        }
        this->move(posX, posY);
    }else{
        showOnTop = false;
        posX = posX > deskRt.x() ? posX : deskRt.x();
        if (posY + mWidHeight > deskRt.height()) {
            posY = mainRect.y() - this->height() - mGap;
            showOnTop = true;
        }
        if(posY < deskRt.y()){
            this->hideToTray(true);
        }else{
            this->hideToTray(false);
            this->move(posX, posY);
        }
    }
    if (mLowWidget) {
        adjustLowWidget();
    }
}

ActionsBar::~ActionsBar()
{
}
