#ifndef FILELISTBUDDY_H
#define FILELISTBUDDY_H
#include <QWidget>
#include <QLabel>
#include <QMenu>
#include <QtWidgets/QMdiArea>
#include <QtWidgets/QListWidget>

//自定义信息Item类
class fileListBuddy : public QWidget
{
    Q_OBJECT
public:
    explicit fileListBuddy(QWidget *parent = 0);
    ~fileListBuddy();

    void initUi(QString filePath,QListWidgetItem *newItem,QListWidget *listWidget,QMdiArea *mdiArea);//初始化Ui
    void contextMenuEvent(QContextMenuEvent*);//菜单事件，为了显示菜单

    QLabel *head;  //头像
    QLabel *name;  //用户名
    QLabel *size;  //用户名
    QLabel *sign;  //个性签名
    QMenu  *menu;

    QString filePath;
    QMdiArea *mdiArea;
    QListWidget *listWidget;
    QListWidgetItem *lwItem;


signals:

public slots:
    void slotDelBuddy();
    void slotOpenMdiArea();

};

#endif // PERSONLISTBUDDY_H
