#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWebSocket>
#include <QWebSocketServer>
#include <QMessageBox>
#include "algoserver.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void webSocketStartListen(qint32 port);

private slots:
    void slotNewWebsocketConn();
    void slotNewWebsocketDisconn();

private:
    Ui::MainWindow *ui;

public:
    QMap<QString, AlgoServer*> algoServerMap;
private:
    QWebSocketServer *_pWebSocketServer;
};
#endif // MAINWINDOW_H
