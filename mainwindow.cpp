#include "filelistbuddy.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTextEdit>
#include <QMdiSubWindow>
#include <QFileInfo>
#include <QScrollBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //设置主窗体
    setWindowTitle("build4eye");
    //TODO:更具显示屏的大小，初始化主窗体各个组件的大小，需要做到等比例放大

    //监听websocket:10080端口，等待算法的链接，后续业务操作考信号与槽
    webSocketStartListen(10080);

    //设置窗体组件样式
    ui->listWidget->horizontalScrollBar()->setStyleSheet("QScrollBar:horizontal"
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
    ui->listWidget->verticalScrollBar()->setStyleSheet("QScrollBar:vertical"
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
        QMessageBox::warning(this, QString::fromUtf8("警告"),QString::fromUtf8("new QWebSocketServer class fail"));
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
        QMessageBox::warning(this, QString::fromUtf8("警告"),QString::fromUtf8("new pWebSocket class fail"));
        return ;
    }

    qDebug() << __FILE__ << __LINE__ << pWebSocket->peerAddress().toString() << pWebSocket->peerPort();

    AlgoServer * algo = new AlgoServer();
    if (algo == NULL){
        QMessageBox::warning(this, QString::fromUtf8("警告"),QString::fromUtf8("new AlgoServer class fail"));
        return ;
    }
    algo->init(pWebSocket);
    algoServerMap.insert(QString("%1-%2").arg(pWebSocket->peerAddress().toString()).arg(pWebSocket->peerPort()) , algo);

    //websocket断开
    connect(pWebSocket, SIGNAL(disconnected()), this, SLOT(slotNewWebsocketDisconn()));
    //TODO hai xu yao clear algo
}

void MainWindow::slotNewWebsocketDisconn()
{
    QWebSocket *pWebSocket = dynamic_cast<QWebSocket *>(sender());
    if(!pWebSocket)
    {
        QMessageBox::warning(this, QString::fromUtf8("警告"),QString::fromUtf8("delete pWebSocket class is null"));
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
    //在资源管理器中，打开并加载文件
    QString s = QFileDialog::getOpenFileName(this,"选择文件","/", "Files(*.*)");
    if(s == NULL) return;

    fileListBuddy *buddy= new fileListBuddy();
    QListWidgetItem *newItem = new QListWidgetItem();       //创建一个newItem
    buddy->initUi(s,newItem,ui->listWidget,ui->mdiArea);

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
