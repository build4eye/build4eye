#include "windowbuddy.h"

#include <QMouseEvent>
#include <QScrollBar>
#include <QDebug>

Windowbuddy::Windowbuddy(QWidget* parent)
    : QScrollArea(parent)
    , mMoveStart(false)
{
    installEventFilter(this);
    horizontalScrollBar()->setStyleSheet("QScrollBar:horizontal"
                                         "{"
                                         "height:8px;"
                                         "background:rgba(0,0,0,0%);"
                                         "margin:0px,0px,0px,0px;"
                                         "padding-left:0px;"
                                         "padding-right:0px;"
                                         "}"
                                         "QScrollBar::handle:horizontal"
                                         "{"
                                         "height:8px;"
                                         "background:rgba(0,0,0,25%);"
                                         " border-radius:4px;"
                                         "min-height:20;"
                                         "}"
                                         "QScrollBar::handle:horizontal:hover"
                                         "{"
                                         "height:8px;"
                                         "background:rgba(0,0,0,50%);"
                                         " border-radius:4px;"
                                         "min-height:20;"
                                         "}"
                                         "QScrollBar::add-line:horizontal"
                                         "{"
                                         "height:0px;width:0px;"
                                         "subcontrol-position:bottom;"
                                         "}"
                                         "QScrollBar::sub-line:horizontal"
                                         "{"
                                         "height:0px;width:0px;"
                                         "subcontrol-position:top;"
                                         "}"
                                         "QScrollBar::add-page:horizontal,QScrollBar::sub-page:horizontal"
                                         "{"
                                         "background:rgba(0,0,0,10%);"
                                         "border-radius:4px;"
                                         "}"
                                         );
    verticalScrollBar()->setStyleSheet("QScrollBar:vertical"
                                       "{"
                                       "width:8px;"
                                       "background:rgba(0,0,0,0%);"
                                       "margin:0px,0px,0px,0px;"
                                       "padding-top:0px;"
                                       "padding-bottom:0px;"
                                       "}"
                                       "QScrollBar::handle:vertical"
                                       "{"
                                       "width:8px;"
                                       "background:rgba(0,0,0,25%);"
                                       " border-radius:4px;"
                                       "min-height:20;"
                                       "}"
                                       "QScrollBar::handle:vertical:hover"
                                       "{"
                                       "width:8px;"
                                       "background:rgba(0,0,0,50%);"
                                       " border-radius:4px;"
                                       "min-height:20;"
                                       "}"
                                       "QScrollBar::add-line:vertical"
                                       "{"
                                       "height:0px;width:0px;"
                                       "subcontrol-position:bottom;"
                                       "}"
                                       "QScrollBar::sub-line:vertical"
                                       "{"
                                       "height:0px;width:0px;"
                                       "subcontrol-position:top;"
                                       "}"
                                       "QScrollBar::add-page:vertical,QScrollBar::sub-page:vertical"
                                       "{"
                                       "background:rgba(0,0,0,10%);"
                                       "border-radius:4px;"
                                       "}"
                                       );
    imageLabel = new QLabel();
    imageLabel->setScaledContents(false);
}

Windowbuddy::~Windowbuddy()
{

}

bool Windowbuddy::eventFilter(QObject *obj, QEvent *evt)
{
    if(evt->type() == QEvent::Wheel)
    {
        QWheelEvent* wheelEvent = (QWheelEvent*) evt;

        if(wheelEvent->delta() > 0 )
        {
            enlargedImage(wheelEvent->pos());
        }
        else
        {
            shrinkImage(wheelEvent->pos());
        }

        return true;
    }

    if(obj == this->verticalScrollBar() || obj == this->horizontalScrollBar())
    {
       return QObject::eventFilter(obj, evt);
    }


    if (evt->type() == QEvent::MouseMove) {
        QMouseEvent* mouseEvent = (QMouseEvent*) evt;
        if ((mouseEvent->buttons() & Qt::LeftButton)) {
            if (!mMoveStart) {
                mMoveStart = true;
                mContinuousMove =false;
                mMousePoint = mouseEvent->globalPos();
            }else {
                QPoint mousePoint = mouseEvent->globalPos();

                int x_offset = mousePoint.y() - mMousePoint.y();
                int y_offset = mousePoint.x() - mMousePoint.x();
                mContinuousMove = true;

                setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);//开启滚动条
                setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

                QScrollBar *vScrollBar=verticalScrollBar();
                QScrollBar *hScrollBar=horizontalScrollBar();

                vScrollBar->setValue(vScrollBar->value() - x_offset);
                hScrollBar->setValue(hScrollBar->value() - y_offset);

                mMousePoint = mousePoint;
            }
            return true;
        }

    }
    else if (evt->type() == QEvent::MouseButtonRelease) {
       mMoveStart = false;
       setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//关闭滚动条
       setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    }

    return QObject::eventFilter(obj, evt);
}

void Windowbuddy::setImage(QImage img)
{
    wheelIndex = 1;

    this->img = img;
    QPixmap pixmap = QPixmap::fromImage(this->img);
    int with = imageLabel->width();
    int height = imageLabel->height();
    QPixmap fitpixmap = pixmap.scaled(with, height, Qt::KeepAspectRatio, Qt::FastTransformation);

    imageLabel->setPixmap(fitpixmap);

    this->setWidget(imageLabel);
}

void Windowbuddy::enlargedImage(QPoint pos)
{
    //FIXME:不知为什么会触发4次事件，而且鼠标的地址有波动
    static int _cnt = 0;
    _cnt = (++_cnt)%4;
    if (_cnt != 0){
        return;
    }

    //计算原来的位置
    QScrollBar *vScrollBar=verticalScrollBar();
    QScrollBar *hScrollBar=horizontalScrollBar();
    int old_x = hScrollBar->value()+pos.x();
    int old_y = vScrollBar->value()+pos.y();
    //计算放大的位置
    old_x = old_x  * 1.2;
    old_y = old_y  * 1.2;

    if(wheelIndex > 10){
        return;
    }else{
        wheelIndex = wheelIndex * 1.2;
    }

    QPixmap pixmap = QPixmap::fromImage(this->img);
    int with = pixmap.width()* wheelIndex;
    int height = pixmap.height()* wheelIndex;
    QPixmap fitpixmap = pixmap.scaled(with, height, Qt::KeepAspectRatio, Qt::FastTransformation);
    imageLabel->setPixmap(fitpixmap);
    imageLabel->setFixedSize(pixmap.size()* wheelIndex);

    //计算新的位置
    hScrollBar->setValue(old_x - pos.x());
    vScrollBar->setValue(old_y- pos.y());
}

void Windowbuddy::shrinkImage(QPoint pos)
{
    //FIXME:不知为什么会触发4次事件，而且鼠标的地址有波动
    static int _cnt = 0;
    _cnt = (++_cnt)%4;
    if (_cnt != 0){
        return;
    }

    //计算原来的位置
    QScrollBar *vScrollBar=verticalScrollBar();
    QScrollBar *hScrollBar=horizontalScrollBar();
    int old_x = hScrollBar->value()+pos.x();
    int old_y = vScrollBar->value()+pos.y();
    //计算放大的位置
    old_x = old_x  * 0.8;
    old_y = old_y  * 0.8;

    if(wheelIndex < 0.1){
        return;
    }else{
        wheelIndex = wheelIndex * 0.8;
    }

    QPixmap pixmap = QPixmap::fromImage(this->img);
    int with = pixmap.width()* wheelIndex;
    int height = pixmap.height()* wheelIndex;
    QPixmap fitpixmap = pixmap.scaled(with, height, Qt::KeepAspectRatio, Qt::FastTransformation);

    imageLabel->setPixmap(fitpixmap);
    imageLabel->setFixedSize(pixmap.size()* wheelIndex);

    //计算新的位置
    hScrollBar->setValue(old_x - pos.x());
    vScrollBar->setValue(old_y- pos.y());
}

