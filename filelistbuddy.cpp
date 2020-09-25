#include "windowbuddy.h"
#include "filelistbuddy.h"
#include "windowform.h"

#include <QPainter>
#include <QAction>
#include <QFileInfo>

fileListBuddy::fileListBuddy(QWidget *parent) :
    QWidget(parent)
{

}

fileListBuddy::~fileListBuddy(){
    this->listWidget->removeItemWidget(this->lwItem);
    delete(this->lwItem);
}

void fileListBuddy::initUi(QString filePath,QListWidgetItem *newItem, QListWidget *listWidget,QMdiArea *mdiArea)
{
    //初始化组件及其布局
    head=new QLabel(this);
    name=new QLabel(this);
    sign=new QLabel(this);
    size=new QLabel(this);
    menu=new QMenu();

    head->setFixedSize(60,60);
    QPalette color;
    color.setColor(QPalette::Text,Qt::gray);
    sign->setPalette(color);
    head->move(7,7);
    name->move(74,10);
    size->move(74,35);
    sign->move(74,55);

    //加载缩放图片，按比例缩放
    QImage image;//转换成QImage
    image.load(filePath);
    QPixmap pixmap = QPixmap::fromImage(image);
    int with = this->head->width();
    int height = this->head->height();
    QPixmap fitpixmap = pixmap.scaled(with, height, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    //获取文件相关信息
    QFileInfo fi;
    QString file_name;
    fi = QFileInfo(filePath);
    file_name = fi.fileName();
    QString image_size;
    image_size = QString::number(image.width())+" x "+QString::number(image.height());

    //设置组件内容
    this->head->setPixmap(fitpixmap);
    this->name->setText(file_name);
    this->size->setText(image_size);
    this->sign->setText(filePath);
    this->filePath = filePath;
    this->lwItem = newItem;
    this->listWidget = listWidget;
    this->mdiArea = mdiArea;

    //添加点击
    QAction *openDraw = new QAction("新窗体", this);
    QAction *copyFile = new QAction("克隆", this);
    QAction *delFile = new QAction("关闭", this);

    menu->addAction(openDraw);
    menu->addAction(copyFile);
    menu->addAction(delFile);

    connect(openDraw,SIGNAL(triggered()),this,SLOT(slotOpenMdiArea()));
    connect(delFile,SIGNAL(triggered()),this,SLOT(slotDelBuddy()));
    connect(copyFile,SIGNAL(triggered()),this,SLOT(slotCopyBuddy()));
}

//菜单事件，为了显示菜单，点击鼠标右键响应，鼠标点击事件mousePressEvent优先于contextMenuEvent
void fileListBuddy::contextMenuEvent(QContextMenuEvent *event)
{
    QWidget::contextMenuEvent(event);           //调用基类事件
    menu->exec(QCursor::pos());
}


void fileListBuddy::slotDelBuddy()
{
    delete(this);
}

void fileListBuddy::slotOpenMdiArea()
{
    QImage image;//转换成QImage
    image.load(filePath);

    windowform *buddy = new windowform;
    buddy->setImage(image);

    this->mdiArea->addSubWindow(buddy);
    buddy->show();
}

void fileListBuddy::slotCopyBuddy()
{
    fileListBuddy *buddy= new fileListBuddy();
    QListWidgetItem *newItem = new QListWidgetItem();       //创建一个newItem
    buddy->initUi(this->filePath,newItem,this->listWidget,this->mdiArea);

    newItem->setSizeHint(QSize(this->listWidget->width() - 5,74));
    this->listWidget->insertItem(this->listWidget->count(),newItem); //将该newItem插入到后面
    this->listWidget->setItemWidget(newItem, buddy); //将buddy赋给该newItem
}




