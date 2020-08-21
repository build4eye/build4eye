#include "mainwindow.h"
#include "ui_mainwindow.h"

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
