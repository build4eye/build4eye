#include "algoserver.h"

AlgoServer::AlgoServer(QObject *parent) : QObject(parent)
{

}

AlgoServer::~AlgoServer()
{

}

void AlgoServer::slotWebsocketTextMessageReceived(const QString &message)
{
//    QWebSocket *pWebSocket = dynamic_cast<QWebSocket *>(sender());
//    if(!pWebSocket)
//    {
//        return;
//    }
//    emit signal_textMessageReceived(pWebSocket->peerAddress().toString(), pWebSocket->peerPort(), message);
    //
    //TODO:完成json格式数据解析
    //TODO:完成raw数据的读取传递

}
