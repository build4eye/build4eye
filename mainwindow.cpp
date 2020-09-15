#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTextEdit>
#include <QMdiSubWindow>
#include <QFileInfo>

#include "fileListBuddy.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("build4eye");

    webSocketStartListen(10080);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slotNewProject()
{

}

void MainWindow::webSocketStartListen(qint32 port)
{
    _pWebSocketServer = new QWebSocketServer("build4eye",  QWebSocketServer::NonSecureMode, 0);
    if(_pWebSocketServer == NULL){
        QMessageBox::information(this, QString::fromUtf8("警告"),QString::fromUtf8("new QWebSocketServer class fail"));
        return ;
    }
    _pWebSocketServer->listen(QHostAddress::Any, port);

    connect(_pWebSocketServer, SIGNAL(newConnection()), this, SLOT(slotNewWebsocketConn()));
}

void MainWindow::slotNewWebsocketConn()
{
    QWebSocket *pWebSocket = _pWebSocketServer->nextPendingConnection();
    if(!pWebSocket)
    {
        return;
    }

    qDebug() << __FILE__ << __LINE__ << pWebSocket->peerAddress().toString() << pWebSocket->peerPort();

    AlgoServer * algo = new AlgoServer();
    if (algo == NULL){
        QMessageBox::information(this, QString::fromUtf8("警告"),QString::fromUtf8("new AlgoServer class fail"));
        return ;
    }

    algoServerMap.insert(QString("%1-%2").arg(pWebSocket->peerAddress().toString()).arg(pWebSocket->peerPort()) , algo);

    algo->pWebSocket = pWebSocket;
    connect(pWebSocket, SIGNAL(disconnected()), this, SLOT(slotNewWebsocketDisconn()));
    connect(pWebSocket, SIGNAL(textMessageReceived(QString)),algo, SLOT(slotWebsocketTextMessageReceived(QString)));
}

void MainWindow::slotNewWebsocketDisconn()
{
    QWebSocket *pWebSocket = dynamic_cast<QWebSocket *>(sender());
    if(!pWebSocket)
    {
        return;
    }
    QString key = QString("%1-%2").arg(pWebSocket->peerAddress().toString())
            .arg(pWebSocket->peerPort());

    if(algoServerMap.contains(key)){
        QMap<QString,AlgoServer*>::iterator it = algoServerMap.find(key);
        delete(it.value());
    }

    algoServerMap.remove(key);

    qDebug() << __FILE__ << __LINE__ << "WebsocketDiscon " << key;
}

void MainWindow::on_openfilebutton_triggered()
{
    QString image_size;
    QString s = QFileDialog::getOpenFileName(this,"选择文件","/", "Files(*.*)");
    QImage image;//转换成QImage
    image.load(s);

    //创建一个文件对象
    QFileInfo fi;
    QString file_name;
    fi = QFileInfo(s);
    file_name = fi.fileName();
    fileListBuddy *buddy= new fileListBuddy();

    QPixmap pixmap = QPixmap::fromImage(image);
    int with = buddy->head->width();
    int height = buddy->head->height();
    QPixmap fitpixmap = pixmap.scaled(with, height, Qt::KeepAspectRatio, Qt::SmoothTransformation);  // 按比例缩放
    image_size = QString::number(image.width())+" x "+QString::number(image.height());

    buddy->head->setPixmap(fitpixmap);
    buddy->name->setText(file_name);
    buddy->size->setText(image_size);
    buddy->sign->setText(s);

    QListWidgetItem *newItem = new QListWidgetItem();       //创建一个newItem
    newItem->setSizeHint(QSize(ui->listWidget->width() - 5,74));
    ui->listWidget->insertItem(ui->listWidget->count(),newItem); //将该newItem插入到后面
    ui->listWidget->setItemWidget(newItem, buddy); //将buddy赋给该newItem


   // QListWidgetItem *newItem=new QListWidgetItem(QIcon(s),file_name);    //创建一个Item
   // newItem->setSizeHint(QSize(ui->listWidget->width()-50,25));//设置宽度、高度
   // ui->listWidget->addItem(newItem);         //加到QListWidget中

    //QListWidgetItem *item = new QListWidgetItem(QIcon(s),file_name);       //创建一个newItem
    //ui->listWidget->insertItem(0,item); //将该newItem插入到后面
    //this->setItemWidget(newItem, buddy); //将buddy赋给该newItem

    //FIXME:下面都是一些零时的代码
//    uchar *buf = (uchar *) malloc(image.height() * image.width() * 4);
//    uchar *tbuf = buf;
//    for(int h = 0; h < image.height(); h++)
//    {
//        for(int w = 0; w < image.width(); w++)
//        {
//            QColor color = image.pixelColor(w, h);
//            *(tbuf++) = (uchar)color.blue();
//            *(tbuf++) = (uchar)color.green();
//            *(tbuf++) = (uchar)color.red();
//            *(tbuf++) = (uchar)color.alpha();
//        }
//    }
//    DataSoruce * ds = new DataSoruce(buf,image.height(),image.width());
//    dataSoruceMap.insert(s, ds);

//    connect(ui->up,SIGNAL(triggered()), ds, SLOT(slotDataUp()));

    //方法1
    QLabel *l = new QLabel;
    l->setPixmap(QPixmap::fromImage(image));
    ui->mdiArea->addSubWindow(l);
    l->show();
    //方法2
    //    QLabel *l = new QLabel;
    //    l->setPixmap(QPixmap::fromImage(image));
    //    QMdiSubWindow *newwin = new QMdiSubWindow;
    //    newwin->resize(200,200);
    //    newwin->setAttribute(Qt::WA_DeleteOnClose);
    //    newwin->setWidget(l);
    //    ui->mdiArea->addSubWindow(newwin);     //将window1放进mdiArea框架
    //    newwin->show();
}

void MainWindow::on_up_triggered()
{

}
//void MainWindow::on_btnNewWindow_clicked()
//{
//    newwin = new QMdiSubWindow;
//    newwin->setWindowTitle("test");
//    newwin->setWidget(&QLabel("hey"));
//    newwin->setAttribute(Qt::WA_DeleteOnClose);
//    newwin->resize(200,200);
//    ui->mdiArea->addSubWindow(newwin);
//    newwin->show();     //在非构造函数不加show()，不显示
//    //setActiveSubWindow(QMdiSubWindow *window);     设置active窗口
//}
