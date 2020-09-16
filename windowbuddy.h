#ifndef WINDOWBUDDY_H
#define WINDOWBUDDY_H

#include <QScrollArea>
#include <QPoint>
#include <QLabel>
#include <QGridLayout>

class Windowbuddy : public QScrollArea
{
    Q_OBJECT

public:
    Windowbuddy(QWidget* parent =NULL);
    ~Windowbuddy();

protected:
    bool eventFilter(QObject *obj, QEvent *evt);

 private:
    bool mMoveStart;
    bool mContinuousMove;
    QPoint mMousePoint;
    double radio = 1;
    float wheelIndex = 0;
    QImage img;


public:
    QLabel *imageLabel;
    void setImage(QImage img);
    void enlargedImage(QPoint pos);
    void shrinkImage(QPoint pos);

};
#endif // WINDOWBUDDY_H
