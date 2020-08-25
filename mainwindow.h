#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWebSocket>
#include <QWebSocketServer>
#include <QMessageBox>
#include <QFileDialog>
#include "datasoruce.h"
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

    void on_openfilebutton_triggered();

    void on_up_triggered();

private:
    Ui::MainWindow *ui;

public:
    QMap<QString, DataSoruce*> dataSoruceMap;
    QMap<QString, AlgoServer*> algoServerMap;
private:
    QWebSocketServer *_pWebSocketServer;

//FIXME:零时的代码
    void creatDataSoruce(QImage *image);
};
#endif // MAINWINDOW_H
