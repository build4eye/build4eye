#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTextEdit>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("build4eye");

    webSocketStartListen(10080);

    //FIXME：暂时指定两个显示图片，以后采用动态
    //window1
 //   QLabel *window1 = ui->label;
 //   ui->mdiArea->addSubWindow(window1);     //将window1放进mdiArea框架

    //window2
    QTextEdit *window2 = new QTextEdit;
    window2->setHtml("C++");
    ui->mdiArea->addSubWindow(window2);

    //window3
    QTextEdit *window3 = new QTextEdit;
    window3->setHtml("Java");
    ui->mdiArea->addSubWindow(window3);

    //window4
    QTextEdit *window4 = new QTextEdit;
    window4->setHtml("Hello World");
    ui->mdiArea->addSubWindow(window4);
}

MainWindow::~MainWindow()
{
    delete ui;
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
    QString s = QFileDialog::getOpenFileName(this,"选择文件","/", "Files(*.*)");

    //FIXME:下面都是一些零时的代码
    QImage image;//转换成QImage
    image.load(s);
    qDebug() << "image.height() = " << image.height();
    qDebug() << "image.width() = " << image.width();
    uchar *buf = (uchar *) malloc(image.height() * image.width() * 4);
    uchar *tbuf = buf;
    for(int h = 0; h < image.height(); h++)
    {
        for(int w = 0; w < image.width(); w++)
        {
            QColor color = image.pixelColor(w, h);
            *(tbuf++) = (uchar)color.blue();
            *(tbuf++) = (uchar)color.green();
            *(tbuf++) = (uchar)color.red();
            *(tbuf++) = (uchar)color.alpha();
        }
    }
    DataSoruce * ds = new DataSoruce(buf,image.height(),image.width());
    dataSoruceMap.insert(s, ds);

    connect(ui->up,SIGNAL(triggered()), ds, SLOT(slotDataUp()));

    creatDataSoruce(&image);
}

//FIXME:下面都是一些零时的代码
void MainWindow::creatDataSoruce(QImage *image)
{
    ui->label->setPixmap(QPixmap::fromImage(*image));
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
