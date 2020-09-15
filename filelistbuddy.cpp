#include "fileListBuddy.h"
#include <QPainter>
#include <QAction>

fileListBuddy::fileListBuddy(QWidget *parent) :
    QWidget(parent)
{
    initUi();
}

//初始化Ui
void fileListBuddy::initUi()
{
    //初始化
    head=new QLabel(this);
    name=new QLabel(this);
    sign=new QLabel(this);
    size=new QLabel(this);
    menu=new QMenu();

    //布局
    head->setFixedSize(60,60);
    QPalette color;
    color.setColor(QPalette::Text,Qt::gray);
    sign->setPalette(color);
    head->move(7,7);
    name->move(74,10);
    size->move(74,35);
    sign->move(74,55);

    //添加点击
    QAction *openDraw = new QAction("打开显示", this);
    QAction *delFile = new QAction("关闭", this);
    menu->addAction(openDraw);
    menu->addAction(delFile);

    //connect(openDraw,SIGNAL(triggered()),this,SLOT(slotAddGroup()));
    //connect(delFile,SIGNAL(triggered()),this,SLOT(slotDelGroup()));
}

//菜单事件，为了显示菜单，点击鼠标右键响应，鼠标点击事件mousePressEvent优先于contextMenuEvent
void fileListBuddy::contextMenuEvent(QContextMenuEvent *event)
{
    QWidget::contextMenuEvent(event);           //调用基类事件
    menu->exec(QCursor::pos());
}


