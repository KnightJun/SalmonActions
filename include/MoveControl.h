#pragma once
#include <qwidget.h>
#include <QPixmap>
#include <QPushButton>
#include "salmonactions_global.h"
enum MoveMode {
    Freely,
    Horizontal,
    Vertical
};
class SALMONACTIONS_EXPORT MoveControl :
    public QPushButton
{
    Q_OBJECT
public:
    MoveControl(QWidget* bindWidget, QWidget* parent = nullptr);
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void setMoveMode(MoveMode mode);
    bool isMoving(){return isPress;};
    virtual void mouseDoubleClickEvent(QMouseEvent *event);
signals:
    void sigMove(const QPoint &pos);
    void sigMoveFinish(const QPoint &pos);
    void sigDoubleClick(QMouseEvent* event);
    void sigMousePress(QMouseEvent* event);
    void sigMouseRelease(QMouseEvent* event);
    
private:
    QWidget* mBindWidget = nullptr;
    MoveMode mMoveMode = Freely;
    QPoint mPressPoint;
    QPoint mPressPos;
    bool isPress = false;
};

