#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("build4eye");

    webSocketStartListen(10080);

    //FIXME：暂时指定两个显示图片，以后采用动态

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


//rgb数据赋值给image转换成pixmap的时间
//total = 0;
//    for(int index = 0; index < _dirs; index++)
//    {
//        for(int index2 = 0; index2 < _count; index2++)
//        {
//            QElapsedTimer t;
//            QString file = QString("./images/images%1/%2.jpg").arg(index+1).arg(index2+1);
//            QImage image;//转换成QImage
//            image.load(file);
//            QByteArray byteArray;
//            for(int h = 0; h < image.height(); h++)
//            {
//                for(int w = 0; w < image.width(); w++)
//                {
//                    QColor color = image.pixelColor(w, h);
////                    qDebug() << __FILE__ << __LINE__ << color.alpha() << (char)color.red() << (char)color.green() << (char)color.blue();
////                    byteArray.append((uchar)color.alpha());
//                    // uchar 输入 gba 给 image当uchar *
////                    、QImage::Format_RGB32，存入格式为B,G,R,A 对应 0,1,2,3
////                        QImage::Format_RGB888，存入格式为R, G, B 对应 0,1,2
////                        QImage::Format_Indexed8，需要设定颜色表，QVector<QRgb>
//                    byteArray.append((uchar)color.blue());
//                    byteArray.append((uchar)color.green());
//                    byteArray.append((uchar)color.red());
//                    byteArray.append((uchar)color.alpha());
////                    if(w == 0 && h == 0)
////                        qDebug() << __FILE__ << __LINE__ << byteArray.toHex();
//                }
//            }
//            t.start();
//            QImage image2((uchar *)byteArray.data(), image.width(), image.height(), QImage::Format_RGB32);
////            QImage image2((uchar *)byteArray.data(), 1, 1, QImage::Format_RGB32);
//            QPixmap bmp;
//            bmp = QPixmap::fromImage(image2);//转换成QPixmap
//            QString str = QString("file: %1 time:%2 width:%3 height:%4").arg(file).arg(t.elapsed()).arg(image2.width()).arg(image2.height());
//            total += t.elapsed();
////            qDebug() << __FILE__ << __LINE__ << image.width() << image.height() << image2.width() << image2.height();
////            image2.pixel(0, 0);
//            if(image.pixel(1000, 1000) == image2.pixel(1000,1000))
//                ui->textBrowser_4->append(str);
////            for(int h = 0; h < image2.height(); h++)
////            {
////                for(int w = 0; w < image2.width(); w++)
////                    ;
////                int w = 1000;
////                qDebug() << __FILE__ << __LINE__ << w << h << image.pixel(w, h) << image2.pixel(w, h) << byteArray.size();
////            }
//                DEBUG(str);
//        }
//    }
//    ui->textBrowser_4->append(QString("total time : %1").arg(total));
