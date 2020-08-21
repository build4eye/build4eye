#ifndef ALGOSERVER_H
#define ALGOSERVER_H

#include <QObject>
#include <QWebSocket>

class AlgoServer : public QObject
{
    Q_OBJECT
public:
    explicit AlgoServer(QObject *parent = nullptr);
    ~AlgoServer();

public slots:
    void slotWebsocketTextMessageReceived(const QString &message);

signals:


public:
    QWebSocket *pWebSocket;

};

#endif // ALGOSERVER_H
