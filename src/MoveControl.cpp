#include <qpainter.h>
#include <QMouseEvent>
#include <SalActIcon.h>
#include "MoveControl.h"

MoveControl::MoveControl(QWidget* bindWidget, QWidget* parent):QPushButton(parent)
{
	this->setIcon(SalActIcon::Move());
	mBindWidget = bindWidget;
	this->setMouseTracking(true);
}

void MoveControl::mousePressEvent(QMouseEvent* event)
{
	emit sigMousePress(event);
	if(event->button() == Qt::LeftButton){
		mPressPoint = event->globalPos();
		if(mBindWidget){
			mPressPos = mBindWidget->pos();
		}
		isPress = true;
	}
}

void MoveControl::mouseReleaseEvent(QMouseEvent* event)
{
	emit sigMouseRelease(event);
	if(event->button() == Qt::LeftButton){
		isPress = false;
		QPoint movePos;
		movePos = mPressPos - (mPressPoint - event->globalPos());
		emit sigMoveFinish(movePos);
	}
}
void MoveControl::mouseDoubleClickEvent(QMouseEvent* event)
{
	emit sigDoubleClick(event);
}
void MoveControl::mouseMoveEvent(QMouseEvent* event)
{
	if (!isPress)return;
	QPoint movePos;
	movePos = mPressPos - (mPressPoint - event->globalPos());
	//mBindWidget->move(movePos);
	switch (mMoveMode) 
	{
	case Horizontal:
		movePos.setY(mPressPos.y());
		break;
	case Vertical:
		movePos.setX(mPressPos.x());
		break;
	default:
		break;
	}
	emit sigMove(movePos);
}

void MoveControl::setMoveMode(MoveMode mode)
{
	mMoveMode = mode;
	switch (mMoveMode)
	{
	case Freely:
		this->setIcon(SalActIcon::Move());
		break;
	case Horizontal:
		this->setIcon(SalActIcon::MoveHorizontal());
		break;
	case Vertical:
		this->setIcon(SalActIcon::MoveVertical());
		break;
	default:
		break;
	}
}
